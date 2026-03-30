/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _HAL_KPCR_H_
#define _HAL_KPCR_H_ 1

#include <sys/types.h>
#include <machine/mcb.h>    /* per-arch */
#include <mm/kalloc.h>

/*
 * Kernel processor control region containing
 * MI processor information
 *
 * @id:  Processor ID
 * @mcb: Machine core block
 * @magwell: Kalloc magazine well
 */
struct kpcr {
    uint16_t id;
    struct mcb mcb;
    struct kalloc_magwell magwell;
};

#endif  /* !_HAL_KPCR_H_ */
