/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <dev/cons/cons.h>
#include <kern/panic.h>
#include <hal/kpcr.h>
#include <acpi/acpi.h>
#include <mm/physmem.h>
#include <mm/vmem.h>
#include <hal/pmap.h>
#include <mm/kalloc.h>
#include <lib/printf.h>
#include <hal/cpu.h>
#include <hal/platform.h>

/* Kernel version */
#define _M1X_VERSION "0.0.2"

static struct kpcr bsp;

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
    hal_cpu_init(&bsp);

    /* Initialize the PMM */
    mm_physmem_init();

    /* Post init the BSP */
    hal_cpu_postinit(&bsp);

    /* Initialize the VMM */
    mm_vmem_init();

    /* Initialize ACPI */
    acpi_init();

    /* Initialize platform specifics */
    hal_platform_init();

    printf("-- END OF KERNEL REACHED; HALTING --\n");
}
