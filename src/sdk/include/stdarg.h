/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _SDK_STDARG_H_
#define _SDK_STDARG_H_ 1

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif      /* __GNUC_VA_LIST */

typedef __gnuc_va_list va_list;

#define va_start(ap, last)  __builtin_va_start((ap), last)
#define va_end(ap)          __builtin_va_end((ap))
#define va_arg(ap, type)    __builtin_va_arg((ap), type)

#endif  /* !_SDK_STDARG_H_ */
