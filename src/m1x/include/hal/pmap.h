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
 * Represents possible pagesizes that can be
 * used.
 *
 * @PAGESIZE_4K:  Use 4 KiB pages [mostly supported]
 * @PAGESIZE_2M:  Use 2 MiB pages [arch specific]
 * @PAGESIZE_1G:  Use 1 GiB pages [very arch specific]
 */
typedef enum {
    PAGESIZE_4K,
    PAGESIZE_2M,
    PAGESIZE_1G
} mmu_pagesize_t;

/*
 * Represents a linear address mapping from virtual
 * to physical and used to create and destroy.
 *
 * @pma: Physical memory address
 * @vma: Virtual memory address
 * @flags: Mapping flags (PROT_*)
 * @ps:    Pagesize to use
 */
struct mmu_mapping {
    uintptr_t pma;
    uintptr_t vma;
    uint16_t flags;
    mmu_pagesize_t ps;
};

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

/*
 * Create a linear address mapping from virtual to physical
 *
 * @vas: Virtual address space to create mapping in
 * @mapping: Mapping descriptor to use
 *
 * Returns zero on success
 */
int hal_map_page(struct vas *vas, struct mmu_mapping *mapping);

#endif  /* !_HAL_PMAP_H_ */
