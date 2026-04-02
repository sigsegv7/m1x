/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/cdefs.h>
#include <sys/mman.h>
#include <hal/pmap.h>
#include <hal/param.h>
#include <lib/printf.h>
#include <mm/physmem.h>
#include <mm/vmem.h>
#include <mm/page.h>
#include <string.h>

#define pr_trace(fmt, ...) \
    printf("pmap: " fmt, ##__VA_ARGS__)

/*
 * Page-Table Entry (PTE) flags
 *
 * See Intel SDM Vol 3A, Section 4.5, Table 4-19
 */
#define PTE_ADDR_MASK   0x000FFFFFFFFFF000
#define PTE_P           BIT(0)        /* Present */
#define PTE_RW          BIT(1)        /* Writable */
#define PTE_US          BIT(2)        /* User r/w allowed */
#define PTE_PWT         BIT(3)        /* Page-level write-through */
#define PTE_PCD         BIT(4)        /* Page-level cache disable */
#define PTE_ACC         BIT(5)        /* Accessed */
#define PTE_DIRTY       BIT(6)        /* Dirty (written-to page) */
#define PTE_PS          BIT(7)        /* Page size */
#define PTE_GLOBAL      BIT(8)        /* Global / sticky */
#define PTE_NX          BIT(63)       /* Execute-disable */

/*
 * Represents the various page table levels
 * that exist
 */
typedef enum {
    PAGEMAP_PML1,
    PAGEMAP_PML2,
    PAGEMAP_PML3,
    PAGEMAP_PML4
} pagemap_level_t;

/*
 * Flush a single entry from the translation look aside buffer. This is
 * used due to the fact that when we overwrite existing mappings, if it
 * were to already be cached, we'd get the old soggy stale mappings that
 * certainly do not reflect our new changes!
 *
 * @vma: Virtual memory address to flush from TLB
 */
static inline void
tlb_flush_single(uintptr_t vma)
{
    __asmv("invlpg (%0)" :: "r" (vma) : "memory");
}

/*
 * Convert a map flags to pte flags
 *
 * @map_flags: Map flags to obtain
 *
 * Returns the PTE flags
 */
static inline uint64_t
pmap_flags_to_pte(uint64_t map_flags)
{
    uint64_t pte_flags = PTE_P | PTE_NX;

    if (ISSET(map_flags, PROT_WRITE))
        pte_flags |= PTE_RW;
    if (ISSET(map_flags, PROT_EXEC))
        pte_flags &= ~PTE_NX;

    return pte_flags;
}

/*
 * Verifies if a given pagesize is supported by this
 * pmap module implementation.
 *
 * @ps: Pagesize to use
 *
 * Returns zero on success, otherwise a non-zero value
 * to indicate and unsupported pagesize.
 */
static inline int
ps_is_valid(mmu_pagesize_t ps)
{
    switch (ps) {
    case PAGESIZE_4K:
        return 0;
    default:
        return -1;
    }
}

/*
 * Obtain a level index from a virtual address and a
 * given level indication
 *
 * @vma: Virtual address to extra level index from
 * @level: Level to extra
 */
static inline uint16_t
pmap_get_index(uintptr_t vma, pagemap_level_t level)
{
    switch (level) {
    case PAGEMAP_PML4: return (vma >> 39) & 0x1FF;
    case PAGEMAP_PML3: return (vma >> 30) & 0x1FF;
    case PAGEMAP_PML2: return (vma >> 21) & 0x1FF;
    case PAGEMAP_PML1: return (vma >> 12) & 0x1FF;
    }

    return 0xFF;
}

/*
 * Extract a specific pagemap level using a virtual address as a key
 * and an indication of the level to extract
 *
 * @vas: Virtual address space to traverse
 * @vma: Virtual memory address used as key
 * @level: Level index to extract
 * @alloc: If true, allocate page
 *
 * Returns the pagemap base on success
 */
static uintptr_t *
pmap_extract_level(struct vas *vas, uintptr_t vma, pagemap_level_t level, bool alloc)
{
    void *tmp;
    struct vas scratch_vas;
    uintptr_t addr, *pagemap = NULL;
    uint16_t index;
    pagemap_level_t cur_level = PAGEMAP_PML4;

    /* Use the current if NULL */
    if (vas == NULL) {
        hal_read_vas(&scratch_vas);
        vas = &scratch_vas;
    }

    pagemap = pma_to_vma((vas->cr3 & PTE_ADDR_MASK));
    while (cur_level > level) {
        index = pmap_get_index(vma, cur_level);

        /* Is this currently present? */
        if (ISSET(pagemap[index], PTE_P)) {
            addr = pagemap[index] & PTE_ADDR_MASK;
            pagemap = pma_to_vma(addr);
            --cur_level;
            continue;
        }

        /* Ok no? Fuck it, we allocate */
        if (!alloc) {
            return NULL;
        }

        /* Allocate a new page for this */
        tmp = mm_alloc_page(1);
        memset(tmp, 0, PAGESIZE);

        /* Set the level */
        addr = vma_to_pma(tmp) | PTE_P | PTE_RW | PTE_US;
        pagemap[index] = addr;

        pagemap = tmp;
        --cur_level;
    }

    return pagemap;
}

void
hal_read_vas(struct vas *res)
{
    if (res == NULL) {
        return;
    }

    __asmv(
        "mov %%cr3, %0"
        : "=r" (res->cr3)
        :
        : "memory"
    );
}

void
hal_write_vas(struct vas *vas)
{
    if (vas == NULL) {
        return;
    }

    __asmv(
        "mov %0, %%cr3"
        :
        : "r" (vas->cr3)
        : "memory"
    );
}

void
hal_fork_vas(struct vas *res)
{
    struct vas new_vas;
    struct vas cur_vas;
    uint64_t *src, *dest;
    uintptr_t cr3;

    if (res == NULL) {
        return;
    }

    hal_read_vas(&cur_vas);
    cr3 = cur_vas.cr3 & PTE_ADDR_MASK;
    new_vas.cr3 = mm_alloc_frame(1);

    src = pma_to_vma(cr3);
    dest = pma_to_vma(new_vas.cr3);

    for (int i = 0; i < 512; ++i) {
        if (i < 256) {
            dest[i] = 0;
        } else {
            dest[i] = src[i];
        }
    }

    *res = new_vas;
}

int
hal_map_page(struct vas *vas, struct mmu_mapping *mapping)
{
    uintptr_t *page_tbl;
    uint16_t index;
    uint64_t flags;

    if (mapping == NULL) {
        return -1;
    }

    /* Is this pagesize even supported? */
    if (ps_is_valid(mapping->ps) != 0) {
        return -1;
    }

    /* What the fuck are you doing? */
    if (mapping->vma == 0) {
        pr_trace("warning: mapping zero is not recommended\n");
        pr_trace("warning: address zero is reserved for trapping errors\n");
    }

    /* Grab the page table */
    page_tbl = pmap_extract_level(vas, mapping->vma, PAGEMAP_PML1, true);
    if (page_tbl == NULL) {
        pr_trace("error: extract level failure on <%p-%p>\n", mapping->vma, mapping->pma);
        return -1;
    }

    index = pmap_get_index(mapping->vma, PAGEMAP_PML1);
    flags = pmap_flags_to_pte(mapping->flags);
    page_tbl[index] |= mapping->pma | flags;
    tlb_flush_single(mapping->vma);
    return 0;
}
