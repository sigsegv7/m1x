/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _CONS_CONS_H_
#define _CONS_CONS_H_ 1

#include <stdint.h>

/*
 * Represents console attributes
 *
 * @fg: Console foreground
 * @bg: Console background
 */
struct cons_attr {
    uint32_t fg;
    uint32_t bg;
};

/*
 * Initialize the boot console
 *
 * @attr: Console attributes
 */
void cons_init(struct cons_attr *attr);

/*
 * Reinitialize the console
 *
 * @attr: Attributes to reinit with
 */
void cons_reinit(struct cons_attr *attr);

/*
 * Write to the boot console
 */
void cons_write(const char *s, size_t len);

#endif  /* !_DRIVER_CONS_CONS_H_ */
