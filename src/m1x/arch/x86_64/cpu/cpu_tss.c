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
    if ((stack_pma = mm_alloc_frame(1)) == 0) {
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
    struct mcb *mcb;
    struct kpcr *self;
    struct tss_entry *tss;
    uintptr_t tss_base;

    if (tss_desc == NULL) {
        return;
    }

    if ((self = hal_this_cpu()) == NULL) {
        panic("tss: could not get current kpcr\n");
    }

    mcb = &self->mcb;
    tss_alloc_resources(mcb);
    tss_base = (uintptr_t)mcb->tss;

    tss_desc->seglimit_low = sizeof(struct tss_entry);
    tss_desc->p = 1;        /* Must be present to be valid */
    tss_desc->g = 0;        /* Granularity -> 0 */
    tss_desc->avl = 0;      /* Not used */
    tss_desc->dpl = 0;      /* Descriptor Privilege Level -> 0 */
    tss_desc->type = 0x9;

    tss_desc->base_low = tss_base & 0xFFFF;
    tss_desc->base_mid = (tss_base >> 16) & 0xFF;
    tss_desc->base_upmid = (tss_base >> 24) & 0xFF;
    tss_desc->base_high = (tss_base >> 32) & 0xFFFFFFFF;

    tss = mcb->tss;
    tss->iobp = 0xFF;       /* No ring 3 I/O port access */
}
