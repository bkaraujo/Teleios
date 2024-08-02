#ifndef TELEIOS_GRAPHICS_EXTERN
#define TELEIOS_GRAPHICS_EXTERN

#include "teleios/types.h"

typedef struct {
    u32 vao;
    u32 shader;
} TLGraphicsState;

extern TLGraphicsState state;

u8 tl_parse_buffer_size(TLBufferType type);
u32 tl_parse_buffer_type(TLBufferType type);
u32 tl_parse_buffer_bytes(TLBufferType type);

u32 tl_parse_geometry_mode(TLGeometryMode mode);

u32 tl_parse_shader_stage(TLShaderStage stage);

u32 tl_parse_texture_target(TLTextureTarget target);
u32 tl_parse_texture_wrap_mode(TLTextureWrapMode mode);
u32 tl_parse_texture_filter_mode(TLTextureFilterMode mode);

#endif // TELEIOS_GRAPHICS_EXTERN