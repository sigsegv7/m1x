/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _HAL_CPU_H_
#define _HAL_CPU_H_ 1

/*
 * Each architecture is to implement their own version
 * of the standard processor primitives.
 */
#include <machine/prim.h>
#include <hal/kpcr.h>

/*
 * Initialize the current processor
 */
void hal_cpu_init(struct kpcr *kpcr);

#endif  /* !_HAL_CPU_H_ */
