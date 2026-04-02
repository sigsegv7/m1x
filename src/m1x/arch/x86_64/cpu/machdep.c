/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <hal/platform.h>
#include <machine/irqchip.h>

void
hal_platform_init(void)
{
    /* Initialize platform IRQ chip */
    md_irqchip_init();
}
