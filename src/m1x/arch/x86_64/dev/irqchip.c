/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/types.h>
#include <sys/param.h>
#include <lib/printf.h>
#include <kern/panic.h>
#include <acpi/tables.h>
#include <acpi/acpi.h>
#include <machine/pio.h>
#include <machine/irqchip.h>

#define pr_trace(fmt, ...) \
    printf("irqchip: " fmt, ##__VA_ARGS__)

/* Presence of i8259s */
#define MADT_PCAT_COMPAT  BIT(0)

/* For keeping track of IRQ chips */
static struct irqchip ioapic_list[IRQCHIP_MAX];
static struct irqchip lapic_list[IRQCHIP_MAX];
static size_t ioapic_count = 0;
static size_t lapic_count = 0;

static inline void
ioapic_add(struct irqchip *chip)
{
    if (ioapic_count >= IRQCHIP_MAX) {
        return;
    }

    ioapic_list[ioapic_count++] = *chip;
    pr_trace(
        "ioapic @ mainbus : [id=%x, gsi_base=%x, base=%x]\n",
        chip->id,
        chip->gsi_base,
        chip->base_addr
    );
}

static inline void
lapic_add(struct irqchip *chip, uint8_t processor_id)
{
    if (lapic_count >= IRQCHIP_MAX) {
        return;
    }

    lapic_list[lapic_count++] = *chip;
    pr_trace(
        "lapic @ core%d : [id=%x, base=%x]\n",
        processor_id,
        chip->id,
        chip->base_addr
    );
}

struct irqchip *
md_ioapic_index(size_t index)
{
    if (index >= ioapic_count) {
        return NULL;
    }

    return &ioapic_list[index];
}

struct irqchip *
md_lapic_index(size_t index)
{
    if (index >= lapic_count) {
        return NULL;
    }

    return &lapic_list[index];
}

int
md_irqchip_init(void)
{
    struct acpi_madt *madt;
    struct local_apic *lapic;
    struct ioapic *ioapic;
    struct apic_header *apichdr;
    struct irqchip chip;
    uint8_t *cur, *end;

    if ((madt = acpi_query("APIC")) == NULL) {
        panic("no acpi madt detected\n");
    }

    /*
     * If there is a dual i8259 setup, disable it in favor for the
     * modern APIC setup.
     */
    if (ISSET(madt->flags, MADT_PCAT_COMPAT)) {
        pr_trace("dual i8259 @ mainboard; disabling for apic\n");
        md_outb(0xA1, 0xFF);
        md_outb(0x21, 0xFF);
    }

    cur = (uint8_t *)(madt + 1);
    end = (uint8_t *)madt + madt->hdr.length;
    while (cur < end) {
        apichdr = (void *)cur;

        switch (apichdr->type) {
        case IRQCHIP_LAPIC:
            lapic = (struct local_apic *)apichdr;
            chip.base_addr = madt->lapic_addr;
            chip.gsi_base = 0;
            chip.id = lapic->apic_id;
            chip.type = IRQCHIP_LAPIC;
            lapic_add(&chip, lapic->processor_id);
            break;
        case IRQCHIP_IOAPIC:
            ioapic = (struct ioapic *)apichdr;
            chip.type = IRQCHIP_IOAPIC;
            chip.id = ioapic->ioapic_id;
            chip.gsi_base = ioapic->gsi_base;
            chip.base_addr = ioapic->ioapic_addr;
            chip.flags = 0;
            ioapic_add(&chip);
            break;

        }

        cur += apichdr->length;
    }

    return 0;
}
