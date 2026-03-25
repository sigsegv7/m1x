/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _SYS_CDEFS_H_
#define _SYS_CDEFS_H_ 1

#define __asmv              __asm__ __volatile__
#define __attr(x)           __attribute__((x))
#define __section(x)        __attr(section((x)))
#define __align(n)          __attr(aligned((n)))
#define __packed            __attr(packed)
#define __no_return         __attr(noreturn)
#define __always_inline     __attr(always_inline)
#define __barrier()         __asmv("" ::: "memory")

#endif  /* !_SYS_CDEFS_H_ */
