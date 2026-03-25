/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

    .text
    .globl _start
    .extern kmain
_start:
    cli
    cld

    xor %rbp, %rbp
    call kmain

1:  hlt
    jmp 1b
