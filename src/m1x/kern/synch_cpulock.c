/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/types.h>
#include <kern/cpulock.h>
#include <hal/prim.h>

void
cpu_lock_acquire(cpu_lock_t *lock, bool irqmut)
{
    bool irq_en = md_cpu_irqen();

    if (lock == NULL) {
        return;
    }

    if (irqmut && irq_en) {
        md_cpu_irqoff();
    }

    while (hal_cpu_aswap(lock, 1))  {
        hal_cpu_spinwait();
    }

    if (irqmut && irq_en) {
        md_cpu_irqon();
    }
}

void
cpu_lock_release(cpu_lock_t *lock)
{
    if (lock == NULL) {
        return;
    }

    hal_cpu_aswap(lock, 0);
}
