/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/param.h>
#include <sys/units.h>
#include <mm/physmem.h>
#include <lib/limine.h>
#include <lib/printf.h>

#define pr_trace(fmt, ...)  \
    printf("pmm: " fmt, ##__VA_ARGS__)

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
    }

    print_size(usable_mem, "avl");
    print_size(total_mem, "total");
}

void
mm_physmem_init(void)
{
    memmap_resp = memmap_req.response;
    physmem_query_map();
}
