/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _SDK_STRING_H_
#define _SDK_STRING_H_ 1

#include <stdint.h>

/*
 * Obtain the length of a string in bytes excluding the NULL
 * terminating character.
 *
 * @s:  String to obtain length from
 *
 * Return values of zero indicate error
 */
size_t strlen(const char *s);

/*
 * Copy memory from `src` to `dest`
 *
 * @dest:  Destination buffer
 * @src:   Source buffer
 * @count: Number of bytes to copy
 *
 * Returns `dest' on success
 */
void *memcpy(void *dest, const void *src, size_t count);

/*
 * Fill a buffer `s` with `n` bytes of `c`
 *
 * @s: Buffer to fill
 * @c: Data to fill with
 * @n: Number of bytes to fill
 *
 * Returns `s` on success
 */
void *memset(void *s, int c, size_t n);

#endif  /* !_SDK_STRING_H_ */
