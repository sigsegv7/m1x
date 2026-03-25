/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#define _M1X_SOURCE
#include <string.h>

void
*memset(void *s, int c, size_t n)
{
    if (s == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < n; ++i) {
        ((uint8_t *)s)[i] = c;
    }

    return s;
}
