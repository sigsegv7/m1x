/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MACHINE_TRAP_H_
#define _MACHINE_TRAP_H_ 1

#ifndef __ASSEMBLER__
#include <machine/frame.h>
#endif  /* !__ASSEMBLER__ */

#define TRAP_DIVERR  0x00       /* Divide error */
#define TRAP_UD      0x01       /* Invalid opcode */
#define TRAP_DF      0x02       /* Double fault */
#define TRAP_TS      0x03       /* Invalid TSS */
#define TRAP_NP      0x04       /* Segment not present */
#define TRAP_SS      0x05       /* Stack segment fault */
#define TRAP_GP      0x06       /* General protection fault */
#define TRAP_PF      0x07       /* Page fault */

#ifndef __ASSEMBLER__
void trap_dispatch(struct trapframe *tf);
#endif  /* !__ASSEMBLER__ */

#endif  /* !_MACHINE_TRAP_H_ */
