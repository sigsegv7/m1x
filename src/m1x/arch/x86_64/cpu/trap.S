/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <machine/trap.h>
#include <machine/frameasm.h>
#include <machine/idt.h>

    .macro set_trap vector, isr, ist
        mov $\vector, %rdi
        lea \isr(%rip), %rsi
        mov $\ist, %rdx
        mov $IDT_TRAP_GATE, %rcx
        call md_idt_gate_set
    .endm

    .text
    .globl md_idt_fill
md_idt_fill:
    push %r12
    push %r13
    push %r14
    push %r15
    push %rbx

    set_trap 0x00, diverr, 0
    set_trap 0x06, ud, 0
    set_trap 0x08, dbflt, 0
    set_trap 0x0A, tsflt, 0
    set_trap 0x0B, npflt, 0

    pop %rbx
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    retq

    .align 8
diverr:
    FRAME_PREAMBLE(TRAP_DIVERR)
    mov %rsp, %rdi
    call trap_dispatch
1:  cli
    hlt
    jmp 1b

ud:
    FRAME_PREAMBLE(TRAP_UD)
    mov %rsp, %rdi
    call trap_dispatch
1:  cli
    hlt
    jmp 1b

dbflt:
    FRAME_PREAMBLE(TRAP_DF)
    mov %rsp, %rdi
    call trap_dispatch
1:  cli
    hlt
    jmp 1b

tsflt:
    FRAME_PREAMBLE(TRAP_TS)
    mov %rsp, %rdi
    call trap_dispatch
1:  cli
    hlt
    jmp 1b

npflt:
    FRAME_PREAMBLE(TRAP_NP)
    mov %rsp, %rdi
    call trap_dispatch
1:  cli
    hlt
    jmp 1b
