/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _SYS_MMAN_H_
#define _SYS_MMAN_H_ 1

#include <sys/param.h>

#define PROT_READ   0x00
#define PROT_WRITE  BIT(0)
#define PROT_EXEC   BIT(1)
#if defined(_KERNEL)
#define PROT_USER   BIT(2)
#endif  /* !_KERNEL */

#endif  /* !_SYS_MMAN_H_ */
