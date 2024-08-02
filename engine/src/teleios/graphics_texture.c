#include "teleios/teleios.h"
#include "teleios/state.h"
#include "glad/glad.h"
#include "stb/image.h"
#include "teleios/graphics_helpers.h"

u32 tl_parse_texture_target(TLTextureTarget target) {
    switch (target) {
        case TL_TEXTURE_2D: return GL_TEXTURE_2D;
        case TL_TEXTURE_3D: return GL_TEXTURE_3D;
    }

    TLFATAL("Unkown TLTextureTarget: %d", target);
}

u32 tl_parse_texture_wrap_mode(TLTextureWrapMode mode) {
    switch (mode) {
        case TL_TEXTURE_WRAP_CLAMP_EDGE: return GL_CLAMP_TO_EDGE;
        case TL_TEXTURE_WRAP_CLAMP_BORDER: return GL_CLAMP_TO_BORDER;
        case TL_TEXTURE_WRAP_REPEAT: return GL_REPEAT;
        case TL_TEXTURE_WRAP_MIRROR_REPEAT: return GL_MIRRORED_REPEAT;
    }

    TLFATAL("Unkown TLTextuTLTextureWrapModereTarget: %d", mode);
}

u32 tl_parse_texture_filter_mode(TLTextureFilterMode mode) {
    switch (mode) {
        case TL_TEXTURE_FILTER_NEAREST: return GL_NEAREST;
        case TL_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
        case TL_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;

        case TL_TEXTURE_FILTER_LINEAR: return GL_LINEAR;
        case TL_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
        case TL_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
    }
    
    TLFATAL("Unkown TLTextureFilterMode: %d", mode);
}

TLTexture* tl_graphics_texture_create(TLTextureCreateInfo* info) {
    TLDPUSH;
    
    if (info == NULL) TLDWRV("TLTextureCreateInfo is NULL", NULL);

    TLTexture* texture = tl_memory_alloc(TL_MEMORY_GRAPHICS_TEXTURE, sizeof(TLTexture));

    glCreateTextures(tl_parse_texture_target(info->target), 1, &texture->handle);
    glTextureParameteri(texture->handle, GL_TEXTURE_WRAP_S, tl_parse_texture_wrap_mode(info->wrap_s));
    glTextureParameteri(texture->handle, GL_TEXTURE_WRAP_T, tl_parse_texture_wrap_mode(info->wrap_t));
    glTextureParameteri(texture->handle, GL_TEXTURE_MIN_FILTER, tl_parse_texture_filter_mode(info->filter_min));
    glTextureParameteri(texture->handle, GL_TEXTURE_MAG_FILTER, tl_parse_texture_filter_mode(info->filter_mag));

    texture->wrap_s = info->wrap_s;
    texture->wrap_t = info->wrap_t;
    texture->filter_min = info->filter_min;
    texture->filter_mag = info->filter_mag;
    texture->mipmap = info->mipmap;

    TLDRV(texture);
}

void tl_graphics_texture_load(TLTextureLoadInfo* info) {
    TLDPUSH;
    TLTexture* texture = info->texture;

    texture->image = tl_resource_image(info->path);
    if (texture->image == NULL) TLDWRE("Failed to load texture image");

    if (texture->size != texture->image->size) {
        // TODO: When should the glTextureStorage2D be recreated
        texture->size = texture->image->size;
        glTextureStorage2D(texture->handle, texture->mipmap, info->storage_format, texture->image->width, texture->image->height);
    }

    glTextureSubImage2D(texture->handle, 0, 0, 0, texture->image->width, texture->image->height, info->image_format, info->type, texture->image->payload);
    stbi_image_free(texture->image->payload);
    texture->image->payload = NULL;
    
    if (texture->mipmap > 1)  {
        glGenerateMipmap(texture->handle);
    }

    TLDRE;
}

TLOVERLOAD void tl_graphics_texture_bind(TLTexture* texture) {
    TLDPUSH;
    tl_graphics_texture_bind(texture, 0);
    TLDRE;
}

TLOVERLOAD void tl_graphics_texture_bind(TLTexture* texture, u8 slot) {
    TLDPUSH;
    if (state.texture == texture->handle) return;
    state.texture = texture->handle;
    glBindTextureUnit(slot, texture->handle);
    TLDRE;
}

void tl_graphics_texture_destroy(TLTexture* texture) {
    TLDPUSH;
    if (texture == NULL) TLDWRE("TLTexture is NULL");

    if (texture->image != NULL) {
        tl_string_free(texture->image->path);
        if (texture->image->payload != NULL) {
            stbi_image_free(texture->image->payload);
        }

        tl_memory_free(TL_MEMORY_GRAPHICS_IMAGE, sizeof(TLImage), texture->image);
        texture->image = NULL;
    }

    tl_memory_free(TL_MEMORY_GRAPHICS_TEXTURE, sizeof(TLTexture), texture);

    TLDRE;
}