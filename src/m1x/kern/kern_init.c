/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <dev/cons/cons.h>
#include <kern/panic.h>
#include <lib/printf.h>
#include <hal/cpu.h>

/* Kernel version */
#define _M1X_VERSION "0.0.1"

/*
 * Display a boot banner on early start up
 */
static void
banner(void)
{
    printf(
        "booting m1x/%s v%s...\n",
        _M1X_ARCH,
        _M1X_VERSION
    );
}

void
kmain(void)
{
    /* Initialize the console */
    cons_init();

    /* Print the boot banner */
    banner();

    /* Initialize the BSP */
    hal_cpu_init();

    panic("end of kernel\n");
    __builtin_unreachable();
}
