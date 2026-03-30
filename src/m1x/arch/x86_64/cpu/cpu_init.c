/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/types.h>
#include <lib/printf.h>
#include <hal/kpcr.h>
#include <machine/idt.h>
#include <machine/mcb.h>
#include <machine/cpuid.h>

#define pr_trace(fmt, ...) \
    printf("cpu: " fmt, ##__VA_ARGS__)

/* From cpu/locore.S */
extern void md_cpu_lgdt(void *gdtr);
extern char g_GDTR[];

static void
cpu_ident(struct mcb *mcb)
{
    uint32_t eax, unused;
    uint8_t model_low, model_high;
    uint8_t family_low, family_high;

    CPUID(1, eax, unused, unused, unused);
    model_low = (eax >> 4) & 0xF;
    model_high = (eax >> 16) & 0xF;
    mcb->model = (model_high << 4) | model_low;

    family_low  = (eax >> 8) & 0xF;
    family_high = (eax >> 20) & 0xFF;
    mcb->family = ((family_high << 4) | family_low);

    pr_trace("model : %x\n", mcb->model);
    pr_trace("family : %x\n", mcb->family);
}

void
hal_cpu_init(struct kpcr *kpcr)
{
    if (kpcr == NULL) {
        return;
    }

    /* Load the GDT */
    md_cpu_lgdt(g_GDTR);

    /* Populate the IDT with entries */
    md_idt_fill();

    /* Load the IDT */
    md_idt_load();

    /* Identify the processor */
    cpu_ident(&kpcr->mcb);
}

void
hal_cpu_postinit(struct kpcr *kpcr)
{
    if (kpcr == NULL) {
        return;
    }

    /* Initialize kalloc */
    mm_kalloc_init(&kpcr->magwell);
}
