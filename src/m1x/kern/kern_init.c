/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <dev/cons/cons.h>
#include <kern/panic.h>
#include <mm/physmem.h>
#include <lib/printf.h>
#include <hal/cpu.h>

/* Kernel version */
#define _M1X_VERSION "0.0.1"

/* Default console attributes */
static struct cons_attr conattr = {
    .fg = 0x008C00,
    .bg = 0x000000
};

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
    cons_init(&conattr);

    /* Print the boot banner */
    banner();

    /* Initialize the BSP */
    hal_cpu_init();

    /* Initialize the PMM */
    mm_physmem_init();

    printf("-- END OF KERNEL REACHED; HALTING --\n");
}
