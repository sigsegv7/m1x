/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <mm/vmem.h>
#include <hal/pmap.h>
#include <lib/limine.h>

static struct vas kvas;
volatile struct limine_hhdm_request hhdm_req = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

int
mm_vmem_init(void)
{
    hal_fork_vas(&kvas);
    hal_write_vas(&kvas);
    return 0;
}
