#ifndef TELEIOS_TYPES
#define TELEIOS_TYPES

#include "stddef.h"
// ############################################################################
//
//                               PLATFORM DETECTOR
// 
// ############################################################################
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   define TLPLATFORM_WINDOWS 1
#   ifndef _WIN64
#       error "64-bit is required on Windows!"
#   endif
#elif defined(__linux__) || defined(__gnu_linux__)
#   define TLPLATFORM_LINUX 1
#   if defined(__ANDROID__)
#       define TLPLATFORM_ANDROID 1
#   endif
#elif defined(__unix__)
#   define TLPLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
#   define TLPLATFORM_POSIX 1
#elif __APPLE__
#   define TLPLATFORM_APPLE 1
#   include <TargetConditionals.h>
#   if TARGET_IPHONE_SIMULATOR
#       define TLPLATFORM_IOS 1
#       define TLPLATFORM_IOS_SIMULATOR 1
#   elif TARGET_OS_IPHONE
#       define TLPLATFORM_IOS 1
#   elif TARGET_OS_MAC
#   else
#   error "Unknown Apple platform"
#   endif
#else
#   error "Unknown platform!"
#endif
// ############################################################################
//
//                               BASIC MACROS
// 
// ############################################################################
#ifdef _MSC_VER
#   define TLINLINE __forceinline
#   define TLNOINLINE __declspec(noinline)
#   ifdef TL_EXPORT
#       define TLAPI __declspec(dllexport)
#   else
#       define TLAPI
#   endif // TL_EXPORT
#elif  defined(__clang__) || defined(__GNUC__)
#   define TLINLINE __attribute__((always_inline)) inline
#   define TLNOINLINE __attribute__((noinline))
#   ifdef TL_EXPORT
#       define TLAPI __attribute__((visibility("default")))
#   else
#       define TLAPI
#   endif // TL_EXPORT
#else
#   define TLAPI
#   define TLINLINE static inline
#   define TLNOINLINE
#endif

#if defined(__clang__) || defined(__GNUC__)
#   define TLSTASSERT _Static_assert
#else
#   define TLSTASSERT static_assert
#endif
// ############################################################################
//
//                               BASIC TYPES
// 
// ############################################################################
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

TLSTASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
TLSTASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
TLSTASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
TLSTASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

#define U8MAX               255
#define U16MAX              65535
#define U32MAX              4294967295
#define U64MAX              18446744073709551615

typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;

#define I8MAX               127
#define I16MAX              32767
#define I32MAX              2147483647
#define I64MAX              9223372036854775807

#define I8MIN               -128
#define I16MIN              -32768
#define I32MIN              -2147483648
#define I64MIN              -9223372036854775808

TLSTASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
TLSTASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
TLSTASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
TLSTASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

typedef float               f32;
typedef double              f64;

TLSTASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
TLSTASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

typedef int                 b32;
typedef char                b8;

#define true  1
#define false 0
// ############################################################################
//
//                               GLM TYPES
// 
// ############################################################################
#include "cglm/types.h"
#include "cglm/types-struct.h"

#endif // TELEIOS_TYPES