/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MM_PHYSMEM_H_
#define _MM_PHYSMEM_H_ 1

#include <sys/types.h>

/*
 * Initialize the physical memory manager
 */
void mm_physmem_init(void);

/*
 * Free frame(s) of memory
 *
 * @base: Base of memory to free
 * @count: Number of frames to free
 */
void pmm_free_frame(uintptr_t base, size_t count);

/*
 * Allocate frame(s) of memory
 *
 * @count: Number of frames to allocate
 *
 * Returns the page aligned base on success, otherwise
 * zero on failure.
 */
uintptr_t pmm_alloc_frame(size_t count);

#endif  /* !_MM_PHYSMEM_H_ */
