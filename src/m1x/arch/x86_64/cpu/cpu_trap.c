/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/types.h>
#include <sys/cdefs.h>
#include <sys/param.h>
#include <lib/printf.h>
#include <kern/panic.h>
#include <machine/trap.h>
#include <machine/pio.h>

/* NMI codes */
#define NMI_WATCHDOG BIT(4)      /* Watchdog */
#define NMI_PARITY  BIT(7)       /* Memory error */
#define NMI_CHANNEL BIT(6)       /* Bus error */

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
    [TRAP_GP]        = "general protection fault",
    [TRAP_PF]        = "page fault"
};

static void
handle_nmi(void)
{
    bool is_fatal = false;
    uint8_t ctl, ctl1;
    char code[] = {
        '-',    /* Watchdog */
        '-',    /* Channel check */
        '-',    /* Parity check */
        '\0'
    };

    ctl = md_inb(0x61);
    ctl1 = md_inb(0x92);

    if (ISSET(ctl, NMI_CHANNEL)) {
        code[0] = 'c';
        is_fatal = true;
    }

    if (ISSET(ctl, NMI_PARITY)) {
        code[1] = 'p';
        is_fatal = true;
    }

    if (ISSET(ctl1, NMI_WATCHDOG)) {
        code[2] = 'w';
    }

    if (is_fatal) {
        panic("got fatal nmi [%s]\n", code);
    }

    printf("got non-maskable interrupt [%s]\n", code);
}

void
trap_dispatch(struct trapframe *tf)
{
    if (tf->trapno == TRAP_NMI) {
        handle_nmi();
        return;
    }

    /* TODO: This should not bring the whole system down */
    panic("got fatal %s\n", TRAPSTR(tf->trapno));
}
