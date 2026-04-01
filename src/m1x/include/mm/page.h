/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MM_PAGE_H_
#define _MM_PAGE_H_ 1

#include <sys/types.h>

/*
 * Allocate one or more virtual pages
 *
 * @count: Number of pages to allocate
 *
 * Returns the page aligned base on success
 */
void *mm_alloc_page(size_t count);

/*
 * Free one or more pages
 *
 * @base: Page base to free at
 * @count: Number of pages to free
 */
void mm_free_page(void *base, size_t count);

#endif /* !_MM_PAGE_H_ */
