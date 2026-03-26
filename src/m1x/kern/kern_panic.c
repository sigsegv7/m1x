/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <kern/panic.h>
#include <lib/printf.h>
#include <hal/cpu.h>

/*
 * Even though the stack is guaranteed to be sane upon entry,
 * it would still be wise to minimize its usage and therefore
 * rely on globals.
 */
static char panic_buf[128];
static va_list panic_ap;

__no_return void
__panic(const char *fmt, ...)
{
    va_start(panic_ap, fmt);
    vsnprintf(panic_buf, sizeof(panic_buf), fmt, panic_ap);
    printf("\033[31;40mpanic\033[0m: %s", panic_buf);

    for (;;) {
        hal_cpu_halt();
    }
}
