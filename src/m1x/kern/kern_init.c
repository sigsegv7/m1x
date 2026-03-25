/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <dev/cons/cons.h>
#include <hal/cpu.h>

void
kmain(void)
{
    /* Initialize the console */
    cons_init();

    /* Initialize the BSP */
    hal_cpu_init();

    for (;;);
}
