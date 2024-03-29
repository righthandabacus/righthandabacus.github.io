---
layout: post
title: "GCC's predefined"
date: 2009-12-21 15:41:08 -0500
tags: code
---

Call with `:|cpp -dM` or the `gcc` version below

```
$ : | gcc -dM -E -x c -
#define __DBL_MIN_EXP__ (-1021)
#define __FLT_MIN__ 1.17549435e-38F
#define __DEC64_DEN__ 0.000000000000001E-383DD
#define __CHAR_BIT__ 8
#define __WCHAR_MAX__ 2147483647
#define __DBL_DENORM_MIN__ 4.9406564584124654e-324
#define __FLT_EVAL_METHOD__ 0
#define __DBL_MIN_10_EXP__ (-307)
#define __FINITE_MATH_ONLY__ 0
#define __DEC64_MAX_EXP__ 384
#define __SHRT_MAX__ 32767
#define __LDBL_MAX__ 1.18973149535723176502e+4932L
#define __APPLE_CC__ 5646
#define __UINTMAX_TYPE__ long unsigned int
#define __DEC32_EPSILON__ 1E-6DF
#define __block __attribute__((__blocks__(byref)))
#define __SCHAR_MAX__ 127
#define __USER_LABEL_PREFIX__ _
#define __STDC_HOSTED__ 1
#define __DEC64_MIN_EXP__ (-383)
#define __DBL_DIG__ 15
#define __FLT_EPSILON__ 1.19209290e-7F
#define __LDBL_MIN__ 3.36210314311209350626e-4932L
#define __DEC32_MAX__ 9.999999E96DF
#define __strong 
#define __APPLE__ 1
#define __DECIMAL_DIG__ 21
#define __LDBL_HAS_QUIET_NAN__ 1
#define __DYNAMIC__ 1
#define __GNUC__ 4
#define __MMX__ 1
#define __FLT_HAS_DENORM__ 1
#define __DBL_MAX__ 1.7976931348623157e+308
#define __DBL_HAS_INFINITY__ 1
#define __DEC32_MIN_EXP__ (-95)
#define OBJC_NEW_PROPERTIES 1
#define __LDBL_HAS_DENORM__ 1
#define __DEC32_MIN__ 1E-95DF
#define __weak __attribute__((objc_gc(weak)))
#define __DBL_MAX_EXP__ 1024
#define __DEC128_EPSILON__ 1E-33DL
#define __SSE2_MATH__ 1
#define __amd64 1
#define __tune_core2__ 1
#define __LONG_LONG_MAX__ 9223372036854775807LL
#define __GXX_ABI_VERSION 1002
#define __FLT_MIN_EXP__ (-125)
#define __x86_64 1
#define __DBL_MIN__ 2.2250738585072014e-308
#define __LP64__ 1
#define __DBL_HAS_QUIET_NAN__ 1
#define __DEC128_MIN__ 1E-6143DL
#define __REGISTER_PREFIX__ 
#define __DBL_HAS_DENORM__ 1
#define __NO_INLINE__ 1
#define __DEC_EVAL_METHOD__ 2
#define __DEC128_MAX__ 9.999999999999999999999999999999999E6144DL
#define __FLT_MANT_DIG__ 24
#define __VERSION__ "4.2.1 (Apple Inc. build 5646) (dot 1)"
#define __DEC64_EPSILON__ 1E-15DD
#define __DEC128_MIN_EXP__ (-6143)
#define __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ 1062
#define __SIZE_TYPE__ long unsigned int
#define __DEC32_DEN__ 0.000001E-95DF
#define __FLT_RADIX__ 2
#define __LDBL_EPSILON__ 1.08420217248550443401e-19L
#define __SSE_MATH__ 1
#define __k8 1
#define __LDBL_DIG__ 18
#define __x86_64__ 1
#define __FLT_HAS_QUIET_NAN__ 1
#define __FLT_MAX_10_EXP__ 38
#define __LONG_MAX__ 9223372036854775807L
#define __FLT_HAS_INFINITY__ 1
#define __DEC64_MAX__ 9.999999999999999E384DD
#define __DEC64_MANT_DIG__ 16
#define __DEC32_MAX_EXP__ 96
#define __DEC128_DEN__ 0.000000000000000000000000000000001E-6143DL
#define __LITTLE_ENDIAN__ 1
#define __LDBL_MANT_DIG__ 64
#define __CONSTANT_CFSTRINGS__ 1
#define __DEC32_MANT_DIG__ 7
#define __k8__ 1
#define __WCHAR_TYPE__ int
#define __pic__ 2
#define __FLT_DIG__ 6
#define __INT_MAX__ 2147483647
#define __FLT_MAX_EXP__ 128
#define __BLOCKS__ 1
#define __DBL_MANT_DIG__ 53
#define __DEC64_MIN__ 1E-383DD
#define __WINT_TYPE__ int
#define __SSE__ 1
#define __LDBL_MIN_EXP__ (-16381)
#define __MACH__ 1
#define __amd64__ 1
#define __LDBL_MAX_EXP__ 16384
#define __SSP__ 1
#define __LDBL_MAX_10_EXP__ 4932
#define __DBL_EPSILON__ 2.2204460492503131e-16
#define _LP64 1
#define __GNUC_PATCHLEVEL__ 1
#define __LDBL_HAS_INFINITY__ 1
#define __INTMAX_MAX__ 9223372036854775807L
#define __FLT_DENORM_MIN__ 1.40129846e-45F
#define __PIC__ 2
#define __FLT_MAX__ 3.40282347e+38F
#define __SSE2__ 1
#define __FLT_MIN_10_EXP__ (-37)
#define __INTMAX_TYPE__ long int
#define __DEC128_MAX_EXP__ 6144
#define __GNUC_MINOR__ 2
#define __DBL_MAX_10_EXP__ 308
#define __LDBL_DENORM_MIN__ 3.64519953188247460253e-4951L
#define __STDC__ 1
#define __PTRDIFF_TYPE__ long int
#define __DEC128_MANT_DIG__ 34
#define __LDBL_MIN_10_EXP__ (-4931)
#define __GNUC_GNU_INLINE__ 1
#define __SSE3__ 1
```
