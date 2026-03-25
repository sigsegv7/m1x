/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <stdint.h>

/* From cpu/locore.S */
extern void md_cpu_lgdt(void *gdtr);
extern char g_GDTR[];

void
hal_cpu_init(void)
{
    /* Load the GDT */
    md_cpu_lgdt(g_GDTR);
}
