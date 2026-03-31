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

    mfence
    lfence
1:  hlt
    jmp 1b

    .globl md_cpu_lgdt
md_cpu_lgdt:
    push %r12
    push %r13
    push %r14
    push %r15
    push %rbx

    lgdt (%rdi)
    push $0x08
    lea 1f(%rip), %rbx
    push %rbx
    lretq

1:  mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    xor %ax, %ax
    mov %ax, %fs
    mov %ax, %gs

    pop %rbx
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    retq

    .globl md_cpu_irqen
md_cpu_irqen:
    pushf
    pop %rax
    test $1<<9, %rax
    jz bad
    mov $1, %rax
    retq
bad:
    mov $0, %rax
    retq

     .section .data
    .globl g_GDT
g_GDT:
.NULL:
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0b00000000
    .byte 0b00000000
    .byte 0x00
.CODE:
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0b10011010
    .byte 0b00100000
    .byte 0x00
.DATA:
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0b10010010
    .byte 0b00000000
    .byte 0x00
.UCODE:
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0b11111010
    .byte 0b10101111
    .byte 0x00
.UDATA:
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0b11110010
    .byte 0b00000000
    .byte 0x00
.TSS:
    .quad 0x00
    .quad 0x00

    .globl g_GDTR
g_GDTR:
    .word g_GDTR - g_GDT
    .quad g_GDT
