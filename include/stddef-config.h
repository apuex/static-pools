#ifndef __APUEX_STD_DEF_CONFIG_INCLUDED_
#define __APUEX_STD_DEF_CONFIG_INCLUDED_

#ifdef HAVE_CSTDINT
#include <cstdint>
#else

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

#endif

typedef uint32_t apuex_bool;

#endif /* __APUEX_STD_DEF_CONFIG_INCLUDED_ */

