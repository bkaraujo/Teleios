#ifndef TELEIOS_GRAPHICS
#define TELEIOS_GRAPHICS

#include "teleios/types.h"
// #####################################################################################################
//
//                                           S H A D E R
//
// #####################################################################################################
typedef struct {
    u32 handle;
    const char* name;
} TLShaderProgram;

TLShaderProgram* tl_graphics_shader_create(TLShaderCreateInfo* specification);
void tl_graphics_shader_destroy(TLShaderProgram* program);
void tl_graphics_shader_bind(TLShaderProgram* program);
void tl_graphics_shader_uniform(TLShaderProgram* program, const char* name, TLBufferType type, void* uniform);
// #####################################################################################################
//
//                                            G E O M E T R Y
//
// #####################################################################################################
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

TLGeometry* tl_graphics_geometry_create(TLGeometryCreateInfo* specification);
void tl_graphics_geometry_destroy(TLGeometry* geometry);
void tl_graphics_geometry_bind(TLGeometry* geometry);
void tl_graphics_geometry_vertices(TLGeometry* geometry, u32 lenght, f32* vertices);
void tl_graphics_geometry_elements_ui(TLGeometry* geometry, u32 lenght, u32* elements);
void tl_graphics_geometry_elements_us(TLGeometry* geometry, u32 lenght, u16* elements);
// #####################################################################################################
//
//                                           T E X T U R E
//
// #####################################################################################################
typedef struct {
    u32 handle;
    const char* name;

    u32 width;
    u32 height;
} TLTexture;

TLTexture* tl_graphics_texture_create(void);
void tl_graphics_texture_destroy(TLTexture* texture);
void tl_graphics_texture_bind(TLTexture* texture);
void tl_graphics_texture_bind_at(TLTexture* texture, u8 slot);
void tl_graphics_texture_load(TLTexture* texture, const char* path);
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