/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

    .text
    .globl panic
    .extern __panic
panic:
    mov $1, %rax                /* Lock ourselves */
    xchg %rax, __mp_sync        /* -> __mp_sync */
    testq $1, %rax              /* Are we another processor? */
    jnz panic_lockout           /* Yes, enter lockout */
    lea stack_top(%rip), %rsp   /* Load a sane stack */
    jmp __panic
panic_lockout:
    cli
    hlt
    jmp panic_lockout

    .section .data
__mp_sync: .quad 0

    .align 8
    .section .bss
stack: .fill 4096, 1, 0
stack_top:
