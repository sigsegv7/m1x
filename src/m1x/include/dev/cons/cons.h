/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _CONS_CONS_H_
#define _CONS_CONS_H_ 1

#include <stdint.h>

/*
 * Initialize the boot console
 */
void cons_init(void);

/*
 * Write to the boot console
 */
void cons_write(const char *s, size_t len);

#endif  /* !_DRIVER_CONS_CONS_H_ */
