#ifndef TELEIOS_GRAPHICS
#define TELEIOS_GRAPHICS

#include "teleios/types.h"
// #####################################################################################################
//
//                                            B U F F E R
//
// #####################################################################################################
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
// #####################################################################################################
//
//                                           S H A D E R
//
// #####################################################################################################
typedef enum {
    TL_SHADER_STAGE_FRAGMENT,
    TL_SHADER_STAGE_COMPUTE,
    TL_SHADER_STAGE_VERTEX,
    TL_SHADER_STAGE_MAXIMUM
} TLShaderStage;

typedef struct {
    const char* name;
    const char* script;
    u32 stage;
} TLShaderSource;

typedef struct {
    u32 handle;
    const char* name;
} TLShaderProgram;

typedef struct {
    const char* name;
    TLShaderSource* sources;
    u8 quantity;
} TLShaderSpecification;

TLShaderProgram* tl_graphics_shader_create(TLShaderSpecification* specification);
void tl_graphics_shader_destroy(TLShaderProgram* program);
void tl_graphics_shader_bind(TLShaderProgram* program);

typedef struct {
    u8 row;
    u8 column;
    f32* values;
} TLUniformMatrix;

void tl_graphics_shader_uniform(TLShaderProgram* program, const char* name, TLBufferType type, void* uniform);
// #####################################################################################################
//
//                                            G E O M E T R Y
//
// #####################################################################################################
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
} TLGeometrySpecification;

typedef struct {
    u32 vao;
    
    u32 vbo;
    u32 vbo_size;
    u32 vbo_stride;

    u32 ebo;
    u32 ebo_size;
    TLBufferType ebo_type;

    TLList* indices;
    TLGeometryMode mode;
} TLGeometry;

TLGeometry* tl_graphics_geometry_create(TLGeometrySpecification* specification);
void tl_graphics_geometry_destroy(TLGeometry* geometry);
void tl_graphics_geometry_bind(TLGeometry* geometry);
void tl_graphics_geometry_vertices(TLGeometry* geometry, u32 lenght, f32* vertices);
void tl_graphics_geometry_elements_ui(TLGeometry* geometry, u32 lenght, u32* elements);
void tl_graphics_geometry_elements_us(TLGeometry* geometry, u32 lenght, u16* elements);
// #####################################################################################################
//
//                                           R E N D E R E R
//
// #####################################################################################################
void tl_graphics_clear(void);
void tl_graphics_draw(TLGeometry* geometry);
// #####################################################################################################
//
//                                           L I F E C Y C L E
//
// #####################################################################################################
b8 tl_graphics_initialize(TLGraphicsCreateInfo* info);
void tl_graphics_update(void);
b8 tl_graphics_terminate(void);

#endif // TELEIOS_GRAPHICS