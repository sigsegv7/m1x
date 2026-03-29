/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/cdefs.h>
#include <hal/pmap.h>
#include <mm/physmem.h>
#include <mm/vmem.h>

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
    new_vas.cr3 = pmm_alloc_frame(1);

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
