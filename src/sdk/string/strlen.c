/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#define _M1X_SOURCE
#include <string.h>

size_t
strlen(const char *s)
{
    size_t length = 0;
    const char *p = s;

    while ((*p++) != '\0') {
        ++length;
    }

    return length;
}
