/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MACHINE_FRAMEASM_H_
#define _MACHINE_FRAMEASM_H_ 1

/*
 * Push all registers onto the stack to construct
 * the trapframe
 */
#define FRAME_PUSHALL(TRAPNO) \
    pushq %rsi          ;\
    pushq %rdi          ;\
    pushq %rcx          ;\
    pushq %rdx          ;\
    pushq %rax          ;\
    pushq %rbp          ;\
    pushq %rbx          ;\
    pushq %r15          ;\
    pushq %r14          ;\
    pushq %r13          ;\
    pushq %r12          ;\
    pushq %r11          ;\
    pushq %r10          ;\
    pushq %r9           ;\
    pushq %r8           ;\
    pushq $TRAPNO

/*
 * Used to restore all registers after an interrupt
 * has completed
 */
#define FRAME_POPALL     \
    add $8, %rsp         ;\
    popq %r8             ;\
    popq %r9             ;\
    popq %r10            ;\
    popq %r11            ;\
    popq %r12            ;\
    popq %r13            ;\
    popq %r14            ;\
    popq %r15            ;\
    popq %rbx            ;\
    popq %rbp            ;\
    popq %rax            ;\
    popq %rdx            ;\
    popq %rcx            ;\
    popq %rdi            ;\
    popq %rsi

/*
 * Used to push the trapframe if the exception has an
 * error code.
 */
#define FRAME_PREAMBLE_EC(TRAPNO) ;\
    testb $3, 16(%rsp)    ;\
    jnz 1f                ;\
    jmp 2f                ;\
1:  lfence                ;\
    swapgs                ;\
2:  FRAME_PUSHALL(TRAPNO)

/*
 * Used to push the trapframe if the exception has no
 * error code.
 */
#define FRAME_PREAMBLE(TRAPNO) ;\
    testb $3, 8(%rsp)     ;\
    jnz 1f                ;\
    jmp 2f                ;\
1:  lfence                ;\
    swapgs                ;\
2:  FRAME_PUSHALL(TRAPNO)

#endif  /* !_MACHINE_FRAMEASM_H_ */
