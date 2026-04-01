/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _MACHINE_MCB_H_
#define _MACHINE_MCB_H_ 1

#include <sys/types.h>
#include <machine/tss.h>
#include <machine/gdt.h>

/*
 * Machine core block containing MD processor
 * information
 *
 * @model:  Processor model ID
 * @family: Processor family ID
 * @tss:    Task state segment
 * @gdt:    Global descriptor table
 * @gdtr:   GDTR descriptor
 */
struct mcb {
    uint8_t model;
    uint16_t family : 12;
    struct tss_entry *tss;
    struct gdt_entry gdt[GDT_ENTRY_COUNT];
    struct gdtr gdtr;
};

#endif  /* !_MACHINE_MCB_H_ */
