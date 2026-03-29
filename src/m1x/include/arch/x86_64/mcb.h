/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MACHINE_MCB_H_
#define _MACHINE_MCB_H_ 1

#include <sys/types.h>
#include <machine/mcb.h>    /* per-arch */

/*
 * Machine core block containing MD processor
 * information
 *
 * @model:  Processor model ID
 * @family: Processor family ID
 */
struct mcb {
    uint8_t model;
    uint16_t family : 12;
};

#endif  /* !_MACHINE_MCB_H_ */
