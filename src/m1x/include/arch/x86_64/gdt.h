/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MACHINE_GDT_H_
#define _MACHINE_GDT_H_ 1

/* Kernel code/data */
#define GDT_KCODE  0x08
#define GDT_KDATA  0x10

/* User code/data */
#define GDT_UCODE  0x18
#define GDT_UDATA  0x20

/* Task state segment */
#define GDT_TSS    0x28
#define GDT_TSS_INDEX 0x05

#endif  /* !_MACHINE_GDT_H_ */
