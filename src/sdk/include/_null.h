#ifndef _SDK_NULL_H_
#define _SDK_NULL_H_ 1

#if !defined(__cplusplus)
#define NULL  ((void *)0)
#else
#if __cplusplus >= 201103L
#define NULL  nullptr
#else
#define NULL ((void *)0)
#endif  /* __cplusplus >= 201103L */
#endif  /* __cplusplus */

#endif  /* !_SDK_NULL_H_ */
