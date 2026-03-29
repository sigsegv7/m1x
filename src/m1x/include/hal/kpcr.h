/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _HAL_KPCR_H_
#define _HAL_KPCR_H_ 1

#include <sys/types.h>
#include <machine/mcb.h>    /* per-arch */

struct kpcr {
    uint16_t id;
    struct mcb mcb;
};

#endif  /* !_HAL_KPCR_H_ */
