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

#endif  /* !_HAL_PRIM_H_ */
