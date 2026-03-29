/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _HAL_PMAP_H_
#define _HAL_PMAP_H_ 1

#include <sys/types.h>
#include <sys/mman.h>
#include <machine/vas.h>    /* per-arch */

/*
 * Obtain the current virtual address space
 *
 * @res: Result is written here
 */
void hal_read_vas(struct vas *res);

/*
 * Fork a virtual address space and only the kernel
 * part
 */
void hal_fork_vas(struct vas *res);

/*
 * Write a new address space
 *
 * @vas: Vas to set
 */
void hal_write_vas(struct vas *vas);

#endif  /* !_HAL_PMAP_H_ */
