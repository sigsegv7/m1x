/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MACHINE_MSR_H_
#define _MACHINE_MSR_H_ 1

#include <sys/types.h>
#include <sys/cdefs.h>

#define IA32_APIC_BASE      0x0000001B
#define IA32_GS_BASE        0xC0000101
#define IA32_MTRR_CAP       0x000000FE
#define IA32_DEF_TYPE       0x000002FF
#define IA32_MTRR_PHYSBASE  0x00000200
#define IA32_MTRR_PHYSMASK  0x00000201
#define IA32_KERNEL_GS_BASE 0xC0000102
#define IA32_EFER           0xC0000080

#ifndef __ASSEMBLER__
static inline void
md_wrmsr(uint32_t msr, uint64_t v)
{
    uint32_t lo, hi;

    lo = v & 0xFFFFFFFF;
    hi = (v >> 32) & 0xFFFFFFFF;

    __asmv(
        "wrmsr"
        :
        : "c" (msr), "a" (lo), "d" (hi)
        : "memory"
    );
}

static inline uint64_t
md_rdmsr(uint32_t msr)
{
    uint32_t lo, hi;

    __asmv(
        "rdmsr"
        : "=a" (lo), "=d" (hi)
        : "c" (msr)
        : "memory"
    );

    return ((uint64_t)hi << 32) | lo;
}

#endif  /* !__ASSEMBLER__ */
#endif  /* !_MACHINE_MSR_H_ */
