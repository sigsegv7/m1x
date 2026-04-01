/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MACHINE_TSS_H_
#define _MACHINE_TSS_H_ 1

#include <sys/types.h>
#include <sys/cdefs.h>

struct __packed tss_desc {
    uint16_t seglimit_low;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t type : 4;
    uint8_t zero : 1;
    uint8_t dpl : 2;
    uint8_t p   : 1;
    uint8_t limit : 4;
    uint8_t avl : 1;
    uint8_t zero1 : 2;
    uint8_t g : 1;
    uint8_t base_upmid;
    uint32_t base_high;
    uint8_t reserved;
    uint8_t zero2 : 5;
    uint32_t reserved1 : 19;
};

struct __packed tss_entry {
    uint32_t reserved;
    uint32_t rsp0_low;
    uint32_t rsp0_high;
    uint32_t rsp1_low;
    uint32_t rsp1_high;
    uint32_t rsp2_low;
    uint32_t rsp2_high;
    uint32_t reserved1;
    uint32_t reserved2;
    uint32_t ist1_low;
    uint32_t ist1_high;
    uint32_t ist2_low;
    uint32_t ist2_high;
    uint32_t ist3_low;
    uint32_t ist3_high;
    uint32_t ist4_low;
    uint32_t ist4_high;
    uint32_t ist5_low;
    uint32_t ist5_high;
    uint32_t ist6_low;
    uint32_t ist6_high;
    uint32_t ist7_low;
    uint32_t ist7_high;
    uint32_t reserved3;
    uint32_t reserved4;
    uint16_t reserved5;
    uint16_t iobp;
};

/*
 * Initialize the task state segment
 *
 * @tss_desc:  Task state segment descriptor
 */
void md_tss_init(struct tss_desc *tss_desc);

#endif  /* !_MACHINE_TSS_H_ */
