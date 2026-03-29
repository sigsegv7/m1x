/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _ACPI_ACPI_H_
#define _ACPI_ACPI_H_ 1

#include <sys/types.h>

/*
 * Initialize the ACPI subsystem
 */
void acpi_init(void);

/*
 * Query for an ACPI table of a specific signature
 */
void *acpi_query(const char *signature);

#endif  /* !_ACPI_ACPI_H_ */
