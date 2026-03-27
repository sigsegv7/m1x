/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _HAL_PARAM_H_
#define _HAL_PARAM_H_ 1

/* Per-arch */
#include <machine/param.h>

/* Assumed pagesize */
#ifndef PAGESIZE
#define PAGESIZE 4096
#endif  /* !PAGESIZE */

/* Assumed coherency unit */
#ifndef COHERENCY_UNIT
#define COHERENCY_UNIT 64
#endif  /* !COHERENCY_UNIT */

#endif  /* !_HAL_PARAM_H_ */
