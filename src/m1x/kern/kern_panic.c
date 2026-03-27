/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <kern/panic.h>
#include <lib/printf.h>
#include <string.h>
#include <dev/cons/cons.h>
#include <hal/cpu.h>

/*
 * Even though the stack is guaranteed to be sane upon entry,
 * it would still be wise to minimize its usage and therefore
 * rely on globals.
 */
static size_t line = 8;
static char panic_buf[128];
static va_list panic_ap;
static struct cons_dimms consdimms;
static struct cons_attr consattr = {
    .fg = 0xFFFFFF,
    .bg = 0x000082
};

static void
panic_putf(const char *fmt, ...)
{
    static char putf_buf[128];
    static size_t width = 0;
    static size_t len = 0;
    static size_t pos = 0;

    va_start(panic_ap, fmt);
    vsnprintf(putf_buf, sizeof(putf_buf), fmt, panic_ap);

    len = strlen(putf_buf);
    width = consdimms.cols;
    pos = ((width - len) / 2);
    printf("\033[%d;%dH%s\n", line++, pos, putf_buf);
}

__no_return void
__panic(const char *fmt, ...)
{
    cons_get_dimms(&consdimms);
    cons_reinit(&consattr);

    panic_putf("\033[?25lA problem has been detected during system operation");
    panic_putf("The machine has been halted to prevent any damange");
    line = 13;

    va_start(panic_ap, fmt);
    vsnprintf(panic_buf, sizeof(panic_buf), fmt, panic_ap);
    panic_putf("Reason: %s", panic_buf);

    for (;;) {
        hal_cpu_halt();
    }
}
