/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MM_VMEM_H_
#define _MM_VMEM_H_ 1

#include <sys/param.h>
#include <lib/limine.h>

#define KERNEL_BASE \
    ((hhdm_req.response)->offset)

/*
 * Helpers used to convert physical to virtual
 * and vice versa.
 */
#define vma_to_pma(vma) \
    PTR_NOFFSET(vma, KERNEL_BASE)
#define pma_to_vma(pma) \
    PTR_OFFSET((void *)pma, KERNEL_BASE)

extern volatile struct limine_hhdm_request hhdm_req;

/*
 * Initialize the virtual memory manager
 */
int mm_vmem_init(void);

#endif  /* !_MM_VMEM_H_ */
