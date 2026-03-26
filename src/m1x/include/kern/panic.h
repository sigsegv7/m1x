/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _KERN_PANIC_H_
#define _KERN_PANIC_H_ 1

#include <sys/cdefs.h>
#include <stdarg.h>

/*
 * Signal to the user that something went seriously wrong and
 * cease all system operation.
 */
__no_return void panic(const char *fmt, ...);

/*
 * Central panic handler
 *
 * XXX: This function should not be called directly! Use the
 *      panic() function instead.
 */
__no_return void __panic(const char *fmt, ...);

#endif  /* !_KERN_PANIC_H_ */
