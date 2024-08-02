#ifndef TELEIOS_GRAPHICS
#define TELEIOS_GRAPHICS

#include "teleios/types.h"
// #####################################################################################################
//
//                                           S H A D E R
//
// #####################################################################################################
TLShaderProgram* tl_graphics_shader_create(TLShaderCreateInfo* specification);
void tl_graphics_shader_destroy(TLShaderProgram* program);
void tl_graphics_shader_bind(TLShaderProgram* program);
void tl_graphics_shader_uniform(TLShaderProgram* program, const char* name, TLBufferType type, void* uniform);
// #####################################################################################################
//
//                                            G E O M E T R Y
//
// #####################################################################################################
TLGeometry* tl_graphics_geometry_create(TLGeometryCreateInfo* specification);
void tl_graphics_geometry_destroy(TLGeometry* geometry);
void tl_graphics_geometry_bind(TLGeometry* geometry);
void tl_graphics_geometry_vertices(TLGeometry* geometry, u32 lenght, f32* vertices);
TLOVERLOAD void tl_graphics_geometry_elements(TLGeometry* geometry, u32 lenght, u32* elements);
TLOVERLOAD void tl_graphics_geometry_elements(TLGeometry* geometry, u32 lenght, u16* elements);
// #####################################################################################################
//
//                                           T E X T U R E
//
// #####################################################################################################

TLTexture* tl_graphics_texture_create(TLTextureCreateInfo* info);
void tl_graphics_texture_destroy(TLTexture* texture);
TLOVERLOAD void tl_graphics_texture_bind(TLTexture* texture);
TLOVERLOAD void tl_graphics_texture_bind(TLTexture* texture, u8 slot);
void tl_graphics_texture_load(TLTextureLoadInfo* info);
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