/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _HAL_PANIC_H_
#define _HAL_PANIC_H_ 1

#include <sys/types.h>

typedef void(*panic_putf_t)(const char *fmt, ...);

/*
 * Used internally by the panic handler, do not call
 * directly.
 */
void hal_panic_trace(panic_putf_t putf);

#endif  /* !_HAL_PANIC_H_ */
