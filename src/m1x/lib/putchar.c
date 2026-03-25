/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <dev/cons/cons.h>

void
_putchar(char c)
{
    cons_write(&c, 1);
}
