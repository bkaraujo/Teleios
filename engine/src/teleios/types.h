#ifndef TELEIOS_TYPES
#define TELEIOS_TYPES

#ifndef __clang__
#   error "Expected CLANG compiler"
#endif

#if __STDC_VERSION__ < 201112L
#   error "C11 not supported"
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

// ############################################################################
//
//                               COMPILER MACROS
// 
// ############################################################################
// #if defined(TELEIOS_BUILD_ALPHA) || defined(TELEIOS_BUILD_BETA)
#   define TLASSERT    _Static_assert
// #else
// #   define TLASSERT    
// #endif

#define TLINLINE     __attribute__((always_inline)) inline
#define TLNOINLINE   __attribute__((noinline))
#define TLOVERLOAD   __attribute__((overloadable))
#ifdef TELEIOS_EXPORT
#    define TLAPI    __attribute__((visibility("default")))
#else
#    define TLAPI
#endif // TELEIOS_EXPORTR

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
//                               BASIC TYPES
// 
// ############################################################################
typedef uint8_t             u8;  // 0 to 255
typedef uint16_t            u16; // 0 to 65_535
typedef uint32_t            u32; // 0 to 429_4967_295
typedef uint64_t            u64; // 0 to 18_446_744_073_709_551_615

TLASSERT(sizeof(u8 ) == 1, "Expected u8 to be 1 byte.");
TLASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
TLASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
TLASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

#define U8MAX               0xff
#define U16MAX              0xffff
#define U32MAX              0xffffffff
#define U64MAX              0xffffffffffffffff

typedef int8_t              i8;  // -128 to 127
typedef int16_t             i16; // -32_768 to 32_767
typedef int32_t             i32; // -2_147_483_648 to 2_147_483_647
typedef int64_t             i64; // -9_223_372_036_854_775_808 to 9_223_372_036_854_775_807

TLASSERT(sizeof(i8 ) == 1, "Expected i8 to be 1 byte.");
TLASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
TLASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
TLASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

#define I8MAX               INT8_MAX
#define I16MAX              INT16_MAX
#define I32MAX              INT32_MAX
#define I64MAX              INT64_MAX

#define I8MIN               INT8_MIN
#define I16MIN              INT16_MIN
#define I32MIN              INT32_MIN
#define I64MIN              INT64_MIN

typedef float               f32;
typedef double              f64;

TLASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
TLASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

typedef int                 b32;
typedef bool                b8;
#endif // TELEIOS_TYPES