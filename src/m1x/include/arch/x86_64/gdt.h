/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MACHINE_GDT_H_
#define _MACHINE_GDT_H_ 1

#ifndef __ASSEMBLER__
#include <sys/types.h>
#include <sys/cdefs.h>
#endif

#define GDT_ENTRY_COUNT 7

/* Kernel code/data */
#define GDT_KCODE  0x08
#define GDT_KDATA  0x10

/* User code/data */
#define GDT_UCODE  0x18
#define GDT_UDATA  0x20

/* Task state segment */
#define GDT_TSS    0x28
#define GDT_TSS_INDEX 0x05

#ifndef __ASSEMBLER__
struct __packed gdt_entry {
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_mid;
    uint16_t attributes;
    uint8_t base_hi;
};

struct __packed gdtr {
    uint16_t limit;
    uintptr_t offset;
};

extern struct gdt_entry g_GDT[GDT_ENTRY_COUNT];
#endif
#endif  /* !_MACHINE_GDT_H_ */
