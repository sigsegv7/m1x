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
 * Console dimensions
 *
 * @cols: Columns
 * @rows: Rows
 */
struct cons_dimms {
    size_t cols;
    size_t rows;
};

/*
 * Initialize the boot console
 *
 * @attr: Console attributes
 */
void cons_init(struct cons_attr *attr);

/*
 * Obtain console dimensions
 *
 * @res: Result is written here
 */
void cons_get_dimms(struct cons_dimms *res);

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
