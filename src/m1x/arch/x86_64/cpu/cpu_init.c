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
#include <machine/msr.h>
#include <machine/tss.h>
#include <string.h>

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

/*
 * Initialize the global descriptor table
 */
static void
cpu_init_gdt(struct mcb *mcb)
{
    struct gdtr *gdtr;

    if (mcb == NULL) {
        return;
    }

    gdtr = &mcb->gdtr;
    memcpy(mcb->gdt, g_GDT, sizeof(mcb->gdt));

    gdtr->limit = sizeof(mcb->gdt) - 1;
    gdtr->offset = (uintptr_t)&mcb->gdt[0];
    md_cpu_lgdt(gdtr);
}

void
hal_cpu_init(struct kpcr *kpcr)
{
    if (kpcr == NULL) {
        return;
    }

    /* Load the GDT */
    cpu_init_gdt(&kpcr->mcb);

    /* Populate the IDT with entries */
    md_idt_fill();

    /* Load the IDT */
    md_idt_load();

    /* Identify the processor */
    cpu_ident(&kpcr->mcb);

    /* Store the current processor in GS */
    md_wrmsr(IA32_GS_BASE, (uintptr_t)kpcr);
}

struct kpcr *
hal_this_cpu(void)
{
    return (void *)md_rdmsr(IA32_GS_BASE);
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
