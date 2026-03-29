/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/types.h>
#include <sys/param.h>
#include <lib/printf.h>
#include <kern/panic.h>
#include <hal/irqchip.h>
#include <acpi/tables.h>
#include <acpi/acpi.h>
#include <machine/pio.h>

#define pr_trace(fmt, ...) \
    printf("irqchip: " fmt, ##__VA_ARGS__)

/* Presence of i8259s */
#define MADT_PCAT_COMPAT  BIT(0)

int
hal_irqchip_init(void)
{
    struct acpi_madt *madt;

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

    return 0;
}
