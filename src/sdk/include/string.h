/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _SDK_STRING_H_
#define _SDK_STRING_H_ 1

#include <sys/types.h>

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

/*
 * Compare two strings and return the differences, zero
 * if equal
 *
 * @s1: String 1 to compare
 * @s2: String 2 to compare
 * @n:  Number of bytes to compare
 *
 * Returns the differences on success
 */
int memcmp(const void *s1, const void *s2, size_t n);

/*
 * Compare to strings and return zero if they are
 * equal.
 */
int strcmp(const char *s1, const char *s2);

#endif  /* !_SDK_STRING_H_ */
