/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MACHINE_PRIM_H_
#define _MACHINE_PRIM_H_ 1

#include <sys/types.h>
#include <sys/cdefs.h>

/* Hint to the processor that we are in a spinloop */
#define md_cpu_spinwait() \
    __asmv("pause" ::: "memory")

/* Halt the current processor core */
#define md_cpu_malt()  \
    __asmv("hlt" ::: "memory")

/*
 * Atomic swap operation
 *
 * @p: Location to swap with `v`
 * @v: Value to swap to `p`
 */
__always_inline static inline size_t
md_cpu_aswap(size_t *p, size_t v)
{
    size_t vret;

    if (p == NULL) {
        return 0;
    }

    vret = *(volatile size_t *)p;
    __asmv(
        "xchg %0, %1\n"
        : "+m" (*p), "+r" (v)
        :
        : "memory"
    );

    return vret;
}

#endif  /* !_MACHINE_PRIM_H_ */
