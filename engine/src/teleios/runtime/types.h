#ifndef TELEIOS_RUNTIME_TYPES
#define TELEIOS_RUNTIME_TYPES

#include "teleios/types.h"

typedef struct TLUlid TLUlid;
typedef struct {
    f32 left;
    f32 right;
    f32 top;
    f32 bottom;
} TLRectangle;

// ############################################################################
//
//                               BASIC MACROS
// 
// ############################################################################
#define TL_NONE             0

#define TLARRLENGTH(array,type) sizeof(array) / sizeof(type)
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
    const u16 year;
    const u16 millis;
    const u8 month;
    const u8 day;
    const u8 hour;
    const u8 minute;
    const u8 seconds;
} TLCalendar;

typedef struct TLTimer TLTimer;
// ############################################################################
//
//                          MESSAGING TYES
// 
// ############################################################################
typedef enum {
    TL_MESSAGE_CHAIN_AVALIABLE,
    TL_MESSAGE_CHAIN_COMSUMED,
    TL_MESSAGE_CHAIN_MAXIMUM
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
    TL_MEMORY_ENGINE_STATE,
    TL_MEMORY_ENGINE_STACK,
    TL_MEMORY_ENGINE_LOGGER,
    TL_MEMORY_ULID,
    TL_MEMORY_TIMER,
    TL_MEMORY_CONTAINER_LIST,
    TL_MEMORY_CONTAINER_LIST_ENTRY,
    TL_MEMORY_CONTAINER_MAP,
    TL_MEMORY_CONTAINER_MAP_ENTRY,
    TL_MEMORY_GRAPHICS_SHADER,
    TL_MEMORY_GRAPHICS_TEXTURE,
    TL_MEMORY_GRAPHICS_GEOMETRY,
    TL_MEMORY_GRAPHICS_IMAGE,
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

typedef union {
    TLUlid* ulid;
    u16 u16;
} TLMapKey;

typedef struct {
    TLMapKey handle;
    TLList* values;
    b8 foo;
} TLMapEntry;

typedef TLList TLMap;
// ############################################################################
//
//                                DIAGNOSTIC TYPES
// 
// ############################################################################
typedef struct {
    const char* function;
    const char* filename;
    u32 linenumber;
} TLStack;
// ############################################################################
//
//                                AUDIO TYPES
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
    TL_TEXTURE_2D,
    TL_TEXTURE_3D
} TLTextureTarget;

typedef enum {
    TL_TEXTURE_WRAP_CLAMP_EDGE,
    TL_TEXTURE_WRAP_CLAMP_BORDER,
    TL_TEXTURE_WRAP_REPEAT,
    TL_TEXTURE_WRAP_MIRROR_REPEAT
} TLTextureWrapMode;

typedef enum {
    TL_TEXTURE_FILTER_NEAREST,
    TL_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR,
    TL_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST,

    TL_TEXTURE_FILTER_LINEAR,
    TL_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR,
    TL_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST,
} TLTextureFilterMode;
typedef struct {
    TLTextureTarget target;
    u8 mipmap;
    TLTextureWrapMode wrap_s;
    TLTextureWrapMode wrap_t;
    TLTextureFilterMode filter_min;
    TLTextureFilterMode filter_mag;
} TLTextureCreateInfo;

typedef struct {
    const char* path;
    u32 width;
    u32 height;
    u32 channels;
    void* payload;
    u64 size;
} TLImage;

typedef struct {
    u32 handle;
    u8 mipmap;
    TLTextureTarget target;
    TLTextureWrapMode wrap_s;
    TLTextureWrapMode wrap_t;
    TLTextureFilterMode filter_min;
    TLTextureFilterMode filter_mag;
    TLImage* image;

    u32 width;
    u32 height;
    u32 storage_format;
} TLTexture;

typedef struct {
    TLTexture* texture;
    const char* path;
    TLBufferType type;
} TLTextureLoadInfo;

typedef struct {
    TLTexture* texture;
    u32 sprites;
    u32 width;
    u32 height;
    u32 space;
} TLSpriteAtlas;

typedef struct {
    TLTexture* texture;
    u32 width;
    u32 height;
    TLRectangle rectangle;
} TLSprite;
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