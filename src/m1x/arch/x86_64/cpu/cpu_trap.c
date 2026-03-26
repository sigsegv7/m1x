/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/param.h>
#include <kern/panic.h>
#include <machine/trap.h>

#define TRAPSTR(TRAPNO)             \
    ((TRAPNO) >= NELEM(trapstr))    \
        ? "bad trap"                \
        : trapstr[(TRAPNO)]

/*
 * A table used to convert trap constants into
 * human readable strings
 */
static const char *trapstr[] = {
    [TRAP_DIVERR]    = "divide error",
    [TRAP_UD]        = "undefined opcode",
    [TRAP_DF]        = "double fault",
    [TRAP_TS]        = "invalid tss",
    [TRAP_NP]        = "segment not present",
    [TRAP_SS]        = "stack segment fault",
    [TRAP_GP]          = "general protection fault",
    [TRAP_PF]        = "page fault"
};

void
trap_dispatch(struct trapframe *tf)
{
    /* TODO: This should not bring the whole system down */
    panic("got fatal %s\n", TRAPSTR(tf->trapno));
}
