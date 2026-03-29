/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _HAL_IRQCHIP_H_
#define _HAL_IRQCHIP_H_ 1

#include <sys/types.h>

#define IRQCHIP_MAX 32

/*
 * Represents the various kinds of IRQ chips
 *
 * @IRQCHIP_LAPIC: This is a Local APIC unit
 * @IRQCHIP_IOAPIC: This is an I/O APIC unit
 */
typedef enum {
    IRQCHIP_LAPIC,
    IRQCHIP_IOAPIC
} irqchip_type_t;

/*
 * Describes a platform IRQ chip
 *
 * @type:       IRQ chip type
 * @id:         IRQ chip ID (assigned by hardware)
 * @gsi_base:   Applies to ACPI and I/O APICs
 * @flags:      Chip specific flags
 * @base_addr:  MMIO base address
 */
struct irqchip {
    irqchip_type_t type;
    uint8_t id;
    uint8_t gsi_base;
    uint32_t flags;
    uintptr_t base_addr;
};

/*
 * Initialize the IRQ chip manager
 */
int hal_irqchip_init(void);

/*
 * Obtain an I/O APIC by index
 *
 * @index: Index of I/O APIC to obtain
 *
 * Returns NULL if not found
 */
struct irqchip *md_ioapic_index(size_t index);

/*
 * Obtain an Local APIC by index
 *
 * @index: Index of Local APIC to obtain
 *
 * Returns NULL if not found
 */
struct irqchip *md_lapic_index(size_t index);

#endif  /* !_HAL_IRQCHIP_H_ */
