/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _KERN_CPULOCK_H_
#define _KERN_CPULOCK_H_ 1

#include <sys/types.h>

typedef size_t cpu_lock_t;

/*
 * Acquire a CPU lock
 *
 * @lock: CPU lock to acquire
 * @irqmut: If true, mutuate the IRQ state
 */
void cpu_lock_acquire(cpu_lock_t *lock, bool irqmut);

/*
 * Relase a CPU lock
 *
 * @lock: Lock to relase
 */
void cpu_lock_release(cpu_lock_t *lock);

#endif  /* !_KERN_CPULOCK_H_ */
