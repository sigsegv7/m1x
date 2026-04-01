/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MM_KALLOC_H_
#define _MM_KALLOC_H_ 1

#include <sys/cdefs.h>
#include <sys/queue.h>
#include <sys/types.h>
#include <kern/cpulock.h>

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
 * @mag:   Magazine we belong to
 * @bitmap: Granularity significant bitmap (1: free)
 * @slab:  Reference to the slab
 */
struct __align(8) kalloc_slab_desc {
    TAILQ_ENTRY(kalloc_slab_desc) link;
    struct kalloc_mag *mag;
    uint64_t bitmap;
    void *slab;
};

/*
 * Represents an allocation header that is tacked
 * before the data.
 *
 * @len: Length of allocation
 * @desc: Slab descriptor
 */
struct kalloc_header {
    size_t len;
    struct kalloc_slab_desc *desc;
};

/*
 * Represents a magazine holding a list of slab
 * descriptors
 *
 * @empty:      Empty slabs
 * @partial:    Partial slabs
 * @full:       Full slabs
 * @gran:       Granularity of this mag
 * @n_entries:  Number of entries in each field
 * @lock:       CPU lock
 */
struct kalloc_mag {
    TAILQ_HEAD(, kalloc_slab_desc) empty;
    TAILQ_HEAD(, kalloc_slab_desc) partial;
    TAILQ_HEAD(, kalloc_slab_desc) full;
    size_t gran;
    size_t n_entries;
    cpu_lock_t lock;
};

/*
 * A magwell holds many power of two magazines
 */
struct kalloc_magwell {
    struct kalloc_mag mag[MAGS_PER_WELL];
};

/*
 * Free a pointer referenced by `ptr`
 *
 * @ptr: Base of region to free
 */
void mm_kfree(void *ptr);

/*
 * Allocate `size` bytes of memory
 *
 * @size: Number of bytes to allocate
 *
 * Returns the base of the memory on success
 */
void *mm_kalloc(size_t size);

/*
 * Resize a buffer to a new given size
 *
 * @ptr: Base of buffer to resize
 * @newsize: New size to provide buffer
 *
 * Returns the base of the new memory on success
 */
void *mm_krealloc(void *ptr, size_t newsize);

/*
 * Initialize a kalloc magazine well
 *
 * @well: magazine well to initialize
 *
 * Returns zero on success
 */
int mm_kalloc_init(struct kalloc_magwell *well);

#endif  /* !_MM_KALLOC_H_ */
