/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MACHINE_VAS_H_
#define _MACHINE_VAS_H_ 1

#include <sys/types.h>

/*
 * Represents a virtual address space
 */
struct vas {
    uint64_t cr3;
};

#endif  /* !_MACHINE_VAS_H_ */
