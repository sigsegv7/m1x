/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <machine/tss.h>
#include <machine/gdt.h>
#include <machine/mcb.h>
#include <mm/kalloc.h>
#include <mm/physmem.h>
#include <mm/vmem.h>
#include <kern/panic.h>
#include <hal/cpu.h>
#include <string.h>

/*
 * Allocate resources associated with the
 * task state segment
 *
 * @mcb:  Machine core block
 */
static void
tss_alloc_resources(struct mcb *mcb)
{
    struct tss_entry *tss;
    uintptr_t stack_pma, stack_vma;

    if (mcb == NULL) {
        return;
    }

    /* Allocate the TSS */
    tss = mm_kalloc(sizeof(struct tss_entry));
    if (tss == NULL) {
        panic("out of memory\n");
    }

    /* Zero everything */
    memset(tss, 0, sizeof(*mcb->tss));

    /* Allocate the kernel stack page */
    if ((stack_pma = pmm_alloc_frame(1)) == 0) {
        panic("out of memory\n");
    }

    /* Encode the kernel stack bits */
    stack_vma = (uintptr_t)pma_to_vma(stack_pma);
    tss->rsp0_low = stack_pma & 0xFFFFFFFF;
    tss->rsp0_high = (stack_pma >> 32) & 0xFFFFFFFF;
    mcb->tss = tss;
}

void
md_tss_init(struct tss_desc *tss_desc)
{
    struct kpcr *self;

    if (tss_desc == NULL) {
        return;
    }

    if ((self = hal_this_cpu()) == NULL) {
        panic("tss: failed to initialize tss\n");
    }

    tss_alloc_resources(&self->mcb);
}
