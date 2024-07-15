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
#elif defined(__clang__) || defined(__GNUC__)
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

#define TLARRLENGTH(array,type) sizeof(array) / sizeof(type)
// ############################################################################
//
//                               BASIC TYPES
// 
// ############################################################################
typedef unsigned char       u8;  // 0 to 255
typedef unsigned short      u16; // 0 to 65_535
typedef unsigned int        u32; // 0 to 429_4967_295
typedef unsigned long long  u64; // 0 to 18_446_744_073_709_551_615

TLSTASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
TLSTASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
TLSTASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
TLSTASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

#define U8MAX               255
#define U16MAX              65535
#define U32MAX              4294967295
#define U64MAX              18446744073709551615

typedef signed char         i8;  // -128 to 127
typedef signed short        i16; // -32_768 to 32_767
typedef signed int          i32; // -2_147_483_648 to 2_147_483_647
typedef signed long long    i64; // -9_223_372_036_854_775_808 to 9_223_372_036_854_775_807

#define I8MAX               127
#define I16MAX              32767
#define I32MAX              2147483647
#define I64MAX              9223372036854775807

#define I8MIN               -128
#define I16MIN              -32768
#define I32MIN              -2147483648
#define I64MIN              -9223372036854775808

TLSTASSERT(sizeof(i8 ) == 1, "Expected i8 to be 1 byte.");
TLSTASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
TLSTASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
TLSTASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

typedef float               f32;
typedef double              f64;

TLSTASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
TLSTASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

typedef int                 b32;
typedef char                b8;

#define true                1
#define false               0
#define TL_NONE             0

typedef struct {
    char ulid[27];
} TLUlid;
// ############################################################################
//
//                               GLM TYPES
// 
// ############################################################################
#include "cglm/types.h"
#include "cglm/types-struct.h"
// ############################################################################
//
//                              LOGGER TYPES
// 
// ############################################################################
typedef enum {
    TL_LOG_LEVEL_FATAL,
    TL_LOG_LEVEL_ERROR,
    TL_LOG_LEVEL_WARN,
    TL_LOG_LEVEL_INFO,
    TL_LOG_LEVEL_DEBUG,
    TL_LOG_LEVEL_TRACE
} TLLogLevel;
// ############################################################################
//
//                              CHRONO TYPES
// 
// ############################################################################
typedef struct {
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 seconds;
    u16 millis;
} TLTime;

typedef struct {
    u64 start;
    u64 update;
} TLTimer;
// ############################################################################
//
//                          MESSAGING TYES
// 
// ############################################################################
typedef enum {
    TL_MESSAGE_AVALIABLE,
    TL_MESSAGE_COMSUMED,
    TL_MESSAGE_MAXIMUM
} TLMessageChain;

typedef union {
    i64 i64[2];
    u64 u64[2];

    f64 f64[2];

    i32 i32[4];
    u32 u32[4];
    f32 f32[4];

    i16 i16[8];

    u16 u16[8];

    i8 i8[16];
    u8 u8[16];

    char c[16];
} TLMessage;
// ############################################################################
//
//                                MEMORY TYPES
// 
// ############################################################################
typedef enum {
    TL_MEMORY_ULID,
    TL_MEMORY_CONTAINER,
    TL_MEMORY_CONTAINER_NODE,
    TL_MEMORY_GRAPHICS,
    TL_MEMORY_FILESYSTEM,
    TL_MEMORY_RESOURCE,
    TL_MEMORY_STRING,
    TL_MEMORY_AUDIO,
    TL_MEMORY_ECS_ENTITY,
    TL_MEMORY_ECS_COMPONENT,
    TL_MEMORY_MAXIMUM
} TLMemoryType;
// ############################################################################
//
//                                CONTAINER TYPES
// 
// ############################################################################
typedef struct TLListNode {
    void* payload;
    struct TLListNode* next;
    struct TLListNode* previous;
} TLListNode;

typedef struct {
    struct TLListNode* head;
    struct TLListNode* tail;
    u32 length;
} TLList;

typedef TLList TLMap;

typedef struct {
    TLUlid* handle;
    TLList* values;
} TLMapEntry;
// ############################################################################
//
//                                DIAGNOSTIC TYPES
// 
// ############################################################################
typedef struct {
    const char* function;
    const char* filename;
    u32 linenumber;
} TLDiagnostic;
// ############################################################################
//
//                                AUDIo TYPES
// 
// ############################################################################
typedef struct {
    u32 handle;
    i32 size;
    i32 channels;
    i32 sample_rate;
    const char* path;
} TLAudioBuffer;

typedef struct {
    u32 handle;
    enum { TL_AUDIO_PLAYING, TL_AUDIO_PAUSED, TL_AUDIO_STOPED } status;
    TLAudioBuffer* buffer;
} TLAudioSource;
// ############################################################################
//
//                                GRAPHICS TYPES
// 
// ############################################################################
typedef enum {
    TL_BUFFER_TYPE_MARIX22,
    TL_BUFFER_TYPE_MARIX23,
    TL_BUFFER_TYPE_MARIX24,

    TL_BUFFER_TYPE_MARIX32,
    TL_BUFFER_TYPE_MARIX33,
    TL_BUFFER_TYPE_MARIX34,

    TL_BUFFER_TYPE_MARIX42,
    TL_BUFFER_TYPE_MARIX43,
    TL_BUFFER_TYPE_MARIX44,

    TL_BUFFER_TYPE_BYTE1,
    TL_BUFFER_TYPE_BYTE2,
    TL_BUFFER_TYPE_BYTE3,
    TL_BUFFER_TYPE_BYTE4,

    TL_BUFFER_TYPE_SHORT1,
    TL_BUFFER_TYPE_SHORT2,
    TL_BUFFER_TYPE_SHORT3,
    TL_BUFFER_TYPE_SHORT4,

    TL_BUFFER_TYPE_INT1,
    TL_BUFFER_TYPE_INT2,
    TL_BUFFER_TYPE_INT3,
    TL_BUFFER_TYPE_INT4,

    TL_BUFFER_TYPE_UBYTE1,
    TL_BUFFER_TYPE_UBYTE2,
    TL_BUFFER_TYPE_UBYTE3,
    TL_BUFFER_TYPE_UBYTE4,

    TL_BUFFER_TYPE_USHORT1,
    TL_BUFFER_TYPE_USHORT2,
    TL_BUFFER_TYPE_USHORT3,
    TL_BUFFER_TYPE_USHORT4,

    TL_BUFFER_TYPE_UINT1,
    TL_BUFFER_TYPE_UINT2,
    TL_BUFFER_TYPE_UINT3,
    TL_BUFFER_TYPE_UINT4,

    TL_BUFFER_TYPE_DOUBLE1,
    TL_BUFFER_TYPE_DOUBLE2,
    TL_BUFFER_TYPE_DOUBLE3,
    TL_BUFFER_TYPE_DOUBLE4,

    TL_BUFFER_TYPE_FLOAT1,
    TL_BUFFER_TYPE_FLOAT2,
    TL_BUFFER_TYPE_FLOAT3,
    TL_BUFFER_TYPE_FLOAT4,

    TL_BUFFER_TYPE_MAXIMUM
} TLBufferType;

typedef enum {
    TL_SHADER_STAGE_FRAGMENT,
    TL_SHADER_STAGE_COMPUTE,
    TL_SHADER_STAGE_VERTEX,
    TL_SHADER_STAGE_MAXIMUM
} TLShaderStage;

typedef struct {
    const char* name;
    const char* script;
    u64 size;
    u32 stage;
} TLShaderSource;

typedef struct {
    const char* name;
    TLShaderSource** sources;
    u8 quantity;
} TLShaderCreateInfo;

typedef struct {
    u32 handle;
    const char* name;
} TLShaderProgram;

typedef struct {
    u8 row;
    u8 column;
    f32* values;
} TLUniformMatrix;

typedef enum {
    TL_GEOMETRY_MODE_TRIANGLES
} TLGeometryMode;

typedef struct {
    const char* name;
    TLBufferType type;
} TLGeometryBuffer;

typedef struct {
    TLGeometryMode mode;
    u8 buffers_length;
    TLGeometryBuffer* buffers;
} TLGeometryCreateInfo;

typedef struct {
    u32 vao;
    
    u32 vbo;
    u32 vbo_length;
    u32 vbo_stride;

    u32 ebo;
    u32 ebo_length;
    TLBufferType ebo_type;

    TLList* indices;
    TLGeometryMode mode;
} TLGeometry;

typedef enum {
    CLAMP_EDGE,
    CLAMP_BORDER,
    REPEAT,
    MIRROR_REPEAT
} TLTextureWrapMode;

typedef enum {
    NEAREST,
    NEAREST_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,

    LINEAR,
    LINEAR_MIPMAP_LINEAR,
    LINEAR_MIPMAP_NEAREST,
} TLTextureFilterMode;

typedef struct {
    TLTextureWrapMode wrap_s;
    TLTextureWrapMode wrap_t;
    TLTextureFilterMode filter_min;
    TLTextureFilterMode filter_mag;
} TLTextureCreateInfo;

typedef struct {
    u32 handle;
    const char* name;

    u32 width;
    u32 height;
} TLTexture;
// ############################################################################
//
//                               GLM TYPES
// 
// ############################################################################

typedef struct {
    const char* title;
    u32 width;
    u32 height;
} TLWindowCreateInfo;

typedef struct {
    vec4s clear_color;
} TLGraphicsCreateInfo;

typedef struct {
    /** Application version information */
    struct {
        u8 major;
        u8 minor;
        u8 patch;
    } version;

    /** Asset lookup folder */
    const char* rootfs;

    /** Window creating parameters */
    TLWindowCreateInfo window;

    /** Graphics API configuraiton parameters */
    TLGraphicsCreateInfo graphics;

} TLAppSpecification;
#endif // TELEIOS_TYPES