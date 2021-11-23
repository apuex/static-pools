#ifndef __APUEX_STD_DEF_CONFIG_INCLUDED_
#define __APUEX_STD_DEF_CONFIG_INCLUDED_

#ifdef _MSC_VER
#include <windows.h>
#endif /*_MSC_VER */

#include <stddef.h>
#if HAVE_CSTDINT==1
#include <stdint.h>
#include <stdbool.h>
#else

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;

#ifndef _MSC_VER
typedef long long          int64_t;
typedef unsigned long long uint64_t;
#else /*_MSC_VER */
typedef INT64              int64_t;
typedef UINT64             uint64_t;
#ifndef __cplusplus
typedef enum { false=0, true=1 } bool;
#endif
#endif /*_MSC_VER */

#endif

typedef enum { apuex_false=0, apuex_true=1 } apuex_bool;

#endif /* __APUEX_STD_DEF_CONFIG_INCLUDED_ */

