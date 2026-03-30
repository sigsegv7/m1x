/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MM_KALLOC_H_
#define _MM_KALLOC_H_ 1

#include <sys/cdefs.h>
#include <sys/queue.h>
#include <sys/types.h>

/* Number of pages in a slab descriptor pool */
#define SLAB_DESC_POOLSZ 1

/* Number of pages per slab */
#define PAGES_PER_SLAB 8

/* Number of magazines per well */
#define MAGS_PER_WELL 8

/*
 * Represents a slab descriptor which references
 * a slab
 *
 * @link:  Queue link
 * @bitmap: Granularity significant bitmap (1: free)
 * @slab:  Reference to the slab
 */
struct __align(8) kalloc_slab_desc {
    TAILQ_ENTRY(kalloc_slab_desc) link;
    uint64_t bitmap;
    void *slab;
};

/*
 * Represents a magazine holding a list of slab
 * descriptors
 *
 * @empty:      Empty slabs
 * @partial:    Partial slabs
 * @full:       Full slabs
 * @n_empty:    Number of empty slabs
 * @n_partial:  Number of partial slabs
 * @n_full:     Number of full slabs
 * @n_entries:  Number of entries in each field
 */
struct kalloc_mag {
    TAILQ_HEAD(, kalloc_slab_desc) empty;
    TAILQ_HEAD(, kalloc_slab_desc) partial;
    TAILQ_HEAD(, kalloc_slab_desc) full;
    size_t n_empty;
    size_t n_partial;
    size_t n_full;
    size_t n_entries;
};

/*
 * A magwell holds many power of two magazines
 */
struct kalloc_magwell {
    struct kalloc_mag mag[MAGS_PER_WELL];
};

/*
 * Allocate a number of bytes from a well
 *
 * @well: Well to allocate from
 * @n:    Number of bytes to allocate
 *
 * Returns the base of the memory on success
 */
void *mm_well_kalloc(struct kalloc_magwell *well, size_t n);

/*
 * Initialize a kalloc magazine well
 *
 * @well: magazine well to initialize
 *
 * Returns zero on success
 */
int mm_kalloc_init(struct kalloc_magwell *well);

#endif  /* !_MM_KALLOC_H_ */
