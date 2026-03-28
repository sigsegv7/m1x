/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/cdefs.h>
#include <hal/panic.h>

/*
 * Print system registers
 */
static void
print_regs(panic_putf_t putf)
{
    uintptr_t cr2, cr3;
    uintptr_t rsp, rbp, rax;
    uintptr_t rdi, rsi, rdx, rcx;

    __asmv(
        "mov %%cr2, %0\n"
        "mov %%cr3, %1\n"
        "mov %%rsp, %2\n"
        "mov %%rbp, %3\n"
        "mov %%rax, %4\n"
        "mov %%rdi, %5\n"
        "mov %%rsi, %6\n"
        "mov %%rdx, %7\n"
        "mov %%rcx, %8\n"
        : "=r" (cr2), "=r" (cr3),
          "=r" (rsp), "=r" (rbp),
          "=r" (rax), "=r" (rdi),
          "=r" (rsi), "=r" (rdx),
          "=r" (rcx)
        :
        : "memory"
    );

    putf("cr2=%p  cr3=%p", cr2, cr3);
    putf("rsp=%p  rbp=%p", rsp, rbp);
    putf("rax=%p  rdi=%p", rax, rdi);
    putf("rsi=%p  rdx=%p", rsi, rdx);
    putf("rcx=%p", rcx);
}

void
hal_panic_trace(panic_putf_t putf)
{
    print_regs(putf);
}
