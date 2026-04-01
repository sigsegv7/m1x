/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/param.h>
#include <sys/units.h>
#include <kern/panic.h>
#include <mm/physmem.h>
#include <mm/vmem.h>
#include <hal/param.h>
#include <lib/limine.h>
#include <lib/printf.h>
#include <string.h>

#define pr_trace(fmt, ...)  \
    printf("pmm: " fmt, ##__VA_ARGS__)

#define MEM_SCAN_START 0x00100000

/* Portability macros */
#define MEMORY_USABLE           (LIMINE_MEMMAP_USABLE)
#define MEMORY_RESERVED         (LIMINE_MEMMAP_RESERVED)
#define MEMORY_ACPI_RECLAIM     (LIMINE_MEMMAP_ACPI_RECLAIMABLE)
#define MEMORY_ACPI_NVS         (LIMINE_MEMMAP_ACPI_NVS)
#define MEMORY_BAD              (LIMINE_MEMMAP_BAD_MEMORY)
#define MEMORY_BOOTLOADER       (LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
#define MEMORY_KERNEL           (LIMINE_MEMMAP_KERNEL_AND_MODULES)
#define MEMORY_FRAMEBUFFER      (LIMINE_MEMMAP_FRAMEBUFFER)
#define MEMORY_ACPI_TABLES      (LIMINE_MEMMAP_ACPI_RECLAIMABLE)

#define MEMORY_MAP_ENTRIES (memmap_resp->entry_count)
#define MEMORY_MAP_INDEX(INDEX) \
    memmap_resp->entries[(INDEX)]

#define MEM_BASE(ENTRY)    \
    ((ENTRY)->base)
#define MEM_LENGTH(ENTRY)  \
    ((ENTRY)->length)
#define MEM_TYPE(ENTRY)    \
    ((ENTRY)->type)

typedef struct limine_memmap_entry mementry_t;

/* Memory map request */
static struct limine_memmap_response *memmap_resp = NULL;
static volatile struct limine_memmap_request memmap_req = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

/* Safe constant to string conversion */
#define MEMTYPE_STR(TYPE)       \
    ((TYPE) < NELEM(typetab))   \
        ? typetab[(TYPE)]       \
        : "bad type"

/*
 * Table used to convert memory type constants into
 * human readable strings
 */
static const char *typetab[] = {
    [MEMORY_USABLE]         = "usable",
    [MEMORY_RESERVED]       = "reserved",
    [MEMORY_ACPI_RECLAIM]   = "acpi reclaimable",
    [MEMORY_ACPI_NVS]       = "acpi nvs",
    [MEMORY_BAD]            = "bad*",
    [MEMORY_BOOTLOADER]     = "bootloader reclaimable",
    [MEMORY_KERNEL]         = "m1x.sys",
    [MEMORY_FRAMEBUFFER]    = "framebuffer",
    [MEMORY_ACPI_TABLES]    = "acpi tables"
};

/* Memory stats */
static size_t usable_mem = 0;
static size_t total_mem = 0;
static size_t bitmap_free_start = 0;
static size_t highest_frame_idx = 0;
static size_t last_idx = 0;
static uintptr_t usable_top = 0;
static size_t bitmap_size = 0;
static uint8_t *bitmap = NULL;

/*
 * Print memory stats in a nice form
 *
 * @len: Length to print
 * @title: Stat title
 */
static inline void
print_size(size_t len, const char *title)
{
    if (title == NULL) {
        return;
    }

    if (len >= UNIT_GIB) {
        pr_trace("%d GiB %s\n", len / UNIT_GIB, title);
    } else if (len >= UNIT_MIB) {
        pr_trace("%d MiB %s\n", len / UNIT_MIB, title);
    } else {
        pr_trace("%d bytes %s\n", len, title);
    }
}

static void
physmem_fill_bitmap(void)
{
    mementry_t *entry;
    uint64_t base, length, type;

    for (size_t i = 0; i < MEMORY_MAP_ENTRIES; ++i) {
        entry = MEMORY_MAP_INDEX(i);
        base = MEM_BASE(entry);
        length = MEM_LENGTH(entry);
        type   = MEM_TYPE(entry);

        if (type != MEMORY_USABLE) {
            continue;
        }

        if (base < MEM_SCAN_START) {
            continue;
        }

        if (bitmap_free_start == 0) {
            bitmap_free_start = base / PAGESIZE;
        }

        for (size_t j = 0; j < length; j += PAGESIZE) {
            CLRBIT(bitmap, (base + j) / PAGESIZE);
        }
    }
}

/*
 * Find a memory hole big enough to store the
 * bitmap
 */
static void
physmem_alloc_bitmap(void)
{
    mementry_t *entry;
    uint64_t base, length, type;

    for (size_t i = 0; i < MEMORY_MAP_ENTRIES; ++i) {
        entry = MEMORY_MAP_INDEX(i);
        base = MEM_BASE(entry);
        length = MEM_LENGTH(entry);
        type   = MEM_TYPE(entry);

        if (type != MEMORY_USABLE) {
            continue;
        }

        if (length < bitmap_size) {
            continue;
        }

        bitmap = pma_to_vma(base);
        memset(bitmap, -1, bitmap_size);
        break;
    }

    memset(bitmap, -1, bitmap_size);
    physmem_fill_bitmap();
}

/*
 * Gather information from the platform memory
 * map
 */
static void
physmem_query_map(void)
{
    mementry_t *entry;
    uint64_t base, length, type;

    for (size_t i = 0; i < MEMORY_MAP_ENTRIES; ++i) {
        entry = MEMORY_MAP_INDEX(i);
        base = MEM_BASE(entry);
        length = MEM_LENGTH(entry);
        type   = MEM_TYPE(entry);

        pr_trace(
            "%p - %p: %s\n",
            base,
            base + length,
            MEMTYPE_STR(type)
        );

        total_mem += length;
        if (type != MEMORY_USABLE) {
            continue;
        }

        usable_mem += length;
        if ((base + length) >= usable_top) {
            usable_top = ALIGN_UP(base + length, PAGESIZE);
        }
    }

    highest_frame_idx = usable_top / PAGESIZE;
    bitmap_size = usable_top / PAGESIZE / 8;

    print_size(usable_mem, "avl");
    print_size(total_mem, "total");
    print_size(bitmap_size, "to bitmap");
    pr_trace("usable top @ %p\n", usable_top);
}

/*
 * Allocate page frames.
 *
 * @count: Number of frames to allocate.
 */
static uintptr_t
__mm_alloc_frame(size_t count)
{
    size_t frames = 0;
    ssize_t idx = -1;
    uintptr_t ret = 0;

    for (size_t i = last_idx; i < highest_frame_idx; ++i) {
        if (!TESTBIT(bitmap, i)) {
            if (idx < 0)
                idx = i;
            if (++frames >= count)
                break;

            continue;
        }

        idx = -1;
        frames = 0;
    }

    if (idx < 0 || frames != count) {
        ret = 0;
        goto done;
    }

    for (size_t i = idx; i < idx + count; ++i) {
        SETBIT(bitmap, i);
    }
    ret = idx * PAGESIZE;
    last_idx = idx;
    memset(pma_to_vma(ret), 0, count * PAGESIZE);
done:
    return ret;
}

/*
 * Central frame allocation routine
 */
uintptr_t
mm_alloc_frame(size_t count)
{
    uintptr_t ret;

    if (count == 0) {
        return 0;
    }

    if ((ret = __mm_alloc_frame(count)) == 0) {
        last_idx = 0;
        ret = __mm_alloc_frame(count);
    }

    return ret;
}

/*
 * Central frame freeing routine
 */
void
mm_free_frame(uintptr_t base, size_t count)
{
    size_t stop_at = base + (count * PAGESIZE);

    base = ALIGN_UP(base, PAGESIZE);
    for (uintptr_t p = base; p < stop_at; p += PAGESIZE) {
        CLRBIT(bitmap, p / PAGESIZE);
    }
}

void
mm_physmem_init(void)
{
    memmap_resp = memmap_req.response;
    physmem_query_map();

    if (usable_mem < UNIT_GIB) {
        panic("m1x requires at least 1 GiB of RAM\n");
    }

    physmem_alloc_bitmap();
}
