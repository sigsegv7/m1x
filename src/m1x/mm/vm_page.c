/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <mm/page.h>
#include <mm/physmem.h>
#include <mm/vmem.h>

void *
mm_alloc_page(size_t count)
{
    uintptr_t pma;

    pma = mm_alloc_frame(count);
    if (pma == 0) {
        return NULL;
    }

    return pma_to_vma(pma);
}

void
mm_free_page(void *base, size_t count)
{
    uintptr_t pma;

    if (base == NULL) {
        return;
    }

    pma = vma_to_pma(base);
    mm_free_frame(pma, count);
}
