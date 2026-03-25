/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MACHINE_IDT_H_
#define _MACHINE_IDT_H_ 1

#ifndef __ASSEMBLER__
#include <sys/types.h>
#include <sys/cdefs.h>
#endif  /* !__ASSEMBLER__ */

#define IDT_INT_GATE    0x8E
#define IDT_TRAP_GATE   0x8F
#define IDT_USER_GATE   0xEE

#ifndef __ASSEMBLER__

/*
 * Represents an interrupt descriptor table gate
 * entry
 */
struct idt_gate {
    uint16_t offset_low;
    uint16_t segment_sel;
    uint8_t ist   : 3;
    uint8_t zero  : 5;
    uint8_t type  : 4;
    uint8_t zero1 : 1;
    uint8_t dpl   : 2;
    uint8_t p     : 1;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
};

/*
 * References an interrupt descriptor table
 */
struct __packed idtr {
    uint16_t limit;
    uintptr_t base;
};

/*
 * Set an entry in the interrupt descriptor table
 *
 * @vector:  Interrupt vector to set
 * @isr:     Interrupt service routine base
 * @ist:     Interrupt stack table
 * @type:    Interrupt gate type
 */
void md_idt_gate_set(uint8_t vector, uintptr_t isr, uint8_t ist, uint8_t type);

/*
 * Load the interrupt descriptor table register
 */
void md_idt_load(void);

#endif  /* !__ASSEMBLER__ */
#endif  /* !_MACHINE_IDT_H_ */
