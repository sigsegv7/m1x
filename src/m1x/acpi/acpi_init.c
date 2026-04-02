/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <kern/panic.h>
#include <lib/limine.h>
#include <lib/printf.h>
#include <hal/irqchip.h>
#include <acpi/acpi.h>
#include <acpi/tables.h>
#include <mm/vmem.h>
#include <string.h>

#define pr_trace(fmt, ...) \
    printf("acpi: " fmt, ##__VA_ARGS__)

static struct acpi_rsdp *rsdp;
static struct acpi_root_sdt *sdt;
static size_t sdt_entries = 0;

static volatile struct limine_rsdp_response *rsdp_resp;
static volatile struct limine_rsdp_request rsdp_req = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

/*
 * Parse the ACPI RSDP
 */
static void
rsdp_parse(void)
{
    uint8_t rev;
    uint8_t csum = 0;

    /*
     * On certain emulators, the revision may appear as zero with
     * functionality equivalent to ACPI 1.0. In such cases with can
     * assume ACPI 1.0.
     */
    if ((rev = rsdp->revision) == 0) {
        ++rev;
    }

    /* Sum all bytes of the table */
    for (size_t i = 0; i < rsdp->length; ++i) {
        csum += ((char *)rsdp)[i];
    }

    /* Is the checksum valid? */
    if (csum != 0) {
        panic("bad acpi rsdp checksum\n");
    }

    pr_trace("detected acpi %d.0 by %.6s\n", rev, rsdp->oemid);
    if (rev < 2) {
        pr_trace("using rsdt as root sdt\n");
        sdt = pma_to_vma((uintptr_t)rsdp->rsdt_addr) ;
        sdt_entries = (sdt->hdr.length - sizeof(sdt->hdr)) / 4;
    } else {
        pr_trace("using xsdt as root sdt\n");
        sdt = pma_to_vma(rsdp->xsdt_addr);
        sdt_entries = (sdt->hdr.length - sizeof(sdt->hdr)) / 8;
    }
}

void *
acpi_query(const char *signature)
{
    struct acpi_header *hdr;
    uintptr_t pma;

    if (signature == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < sdt_entries; ++i) {
        pma = (uintptr_t)sdt->tables[i];
        hdr = (struct acpi_header *)pma_to_vma(pma);
        if (memcmp(hdr->signature, signature, sizeof(hdr->signature)) == 0) {
            return (void *)hdr;
        }
    }

    return NULL;
}

void
acpi_init(void)
{
    if ((rsdp_resp = rsdp_req.response) == NULL) {
        panic("machine is not acpi-enabled\n");
    }

    rsdp = rsdp = rsdp_resp->address;
    rsdp_parse();
    hal_irqchip_init();
}
