#ifndef TELEIOS_GRAPHICS
#define TELEIOS_GRAPHICS

#include "teleios/types.h"
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

typedef enum {
    TL_SHADER_UNIFORM_MARIX22,
    TL_SHADER_UNIFORM_MARIX23,
    TL_SHADER_UNIFORM_MARIX24,

    TL_SHADER_UNIFORM_MARIX32,
    TL_SHADER_UNIFORM_MARIX33,
    TL_SHADER_UNIFORM_MARIX34,

    TL_SHADER_UNIFORM_MARIX42,
    TL_SHADER_UNIFORM_MARIX43,
    TL_SHADER_UNIFORM_MARIX44,

    TL_SHADER_UNIFORM_FLOAT1,
    TL_SHADER_UNIFORM_FLOAT2,
    TL_SHADER_UNIFORM_FLOAT3,
    TL_SHADER_UNIFORM_FLOAT4,

    TL_SHADER_UNIFORM_INT1,
    TL_SHADER_UNIFORM_INT2,
    TL_SHADER_UNIFORM_INT3,
    TL_SHADER_UNIFORM_INT4
} TLShaderUniformType;

typedef struct {
    u8 row;
    u8 column;
    f32* values;
} TLUniformMatrix;

void tl_graphics_shader_uniform(TLShaderProgram* program, const char* name, TLShaderUniformType type, void* uniform);
// #####################################################################################################
//
//                                            G E O M E T R Y
//
// #####################################################################################################

// #####################################################################################################
//
//                                           L I F E C Y C L E
//
// #####################################################################################################
b8 tl_graphics_initialize(void);
void tl_graphics_update(void);
b8 tl_graphics_terminate(void);

#endif // TELEIOS_GRAPHICS