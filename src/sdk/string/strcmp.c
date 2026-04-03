/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <string.h>

int
strcmp(const char *s1, const char *s2)
{
    const char *p, *p1;

    if (s1 == NULL || s2 == NULL) {
        return 1;
    }

    p = s1;
    p1 = s2;

    while (*p != '\0') {
        if (*p1 == '\0') {
            return -1;
        }

        if (*p != *p1) {
            return -1;
        }

        ++p, ++p1;
    }
}
