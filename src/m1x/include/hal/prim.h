/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _HAL_PRIM_H_
#define _HAL_PRIM_H_ 1

#include <machine/prim.h>   /* per-arch */

#ifndef hal_cpu_spinwait
#define hal_cpu_spinwait md_cpu_spinwait
#endif  /* !hal_cpu_spinwait */

#ifndef hal_cpu_halt
#define hal_cpu_halt md_cpu_halt
#endif  /* !hal_cpu_halt */

#ifndef hal_cpu_aswap
#define hal_cpu_aswap md_cpu_aswap
#endif  /* !hal_cpu_aswap */

#ifndef hal_cpu_irqoff
#define hal_cpu_irqoff md_cpu_irqoff
#endif  /* !hal_cpu_irqoff */

#ifndef hal_cpu_irqon
#define hal_cpu_irqon md_cpu_irqon
#endif  /* !hal_cpu_irqon */

#ifndef hal_cpu_irqen
#define hal_cpu_irqen md_cpu_irqen
#endif  /* !hal_cpu_irqen */

#endif  /* !_HAL_PRIM_H_ */
