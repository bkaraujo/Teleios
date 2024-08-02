#include "teleios/teleios.h"
#include "teleios/state.h"
#include "glad/glad.h"
#include "stb/image.h"
#include "teleios/graphics_helpers.h"

u32 tl_parse_geometry_mode(TLGeometryMode mode) {
    switch (mode) {
        case TL_GEOMETRY_MODE_TRIANGLES: return GL_TRIANGLES;
    }

    TLFATAL("Failed to parse TLGeometryMode");
}

TLGeometry* tl_graphics_geometry_create(TLGeometryCreateInfo* info) {
    TLDPUSH;
    if (info == NULL) TLDWRV("TLGeometryCreateInfo is NULL", NULL);
    if (info->buffers_length == 0) TLDWRV("TLGeometryCreateInfo->buffers_length == 0", NULL);
    if (info->buffers == NULL) TLDWRV("TLGeometryCreateInfo->buffers is NULL", NULL);

    TLGeometry* geometry = tl_memory_alloc(TL_MEMORY_GRAPHICS_GEOMETRY, sizeof(TLGeometry));
    if (geometry == NULL) { TLFATAL("Failed to allocate TLGeometry"); }

    u32 offset = 0;
    glCreateVertexArrays(1, &geometry->vao);
    for (u8 i = 0; i < info->buffers_length ; ++i) {
        glEnableVertexArrayAttrib(geometry->vao, i);
        TLGeometryBuffer buffer = info->buffers[i];
        glVertexArrayAttribFormat(geometry->vao, i, tl_parse_buffer_size(buffer.type), tl_parse_buffer_type(buffer.type), GL_FALSE, offset); 
        glVertexArrayAttribBinding(geometry->vao, i, 0);
        offset += tl_parse_buffer_bytes(buffer.type);
        geometry->vbo_stride += offset;
    }

	TLDRV(geometry);
}

static void tl_graphics_geometry_elements(TLGeometry* geometry, TLBufferType type, u32 lenght, void* elements) {
    TLDPUSH;
    
    if(geometry == NULL) TLDWRE("TLGeometry is NULL");
    u32 bytes = tl_parse_buffer_bytes(type);
    u32 size = lenght * bytes;

    // Expand the buffer to acomodate the elements
    if (lenght > geometry->ebo_length || geometry->ebo_type != type) {
        TLTRACE("TLGeometry[vao %d] Expanding EBO from %lu to %lu bytes", geometry->vao, geometry->ebo_length * bytes, size);
        if (geometry->ebo != GL_NONE) { glDeleteBuffers(1, &geometry->ebo); }
        glCreateBuffers(1, &geometry->ebo);
        glVertexArrayElementBuffer(geometry->vao, geometry->ebo);
        glNamedBufferStorage(geometry->ebo, size, NULL, GL_DYNAMIC_STORAGE_BIT);
        geometry->ebo_type = type;
        geometry->ebo_length = lenght;
    }

    // Push the date into the GPU
    glNamedBufferSubData(geometry->ebo, 0, size, elements);

	TLDRE;
}

TLOVERLOAD void tl_graphics_geometry_elements(TLGeometry* geometry, u32 lenght, u32* elements) {
    TLDPUSH;

    tl_graphics_geometry_elements(geometry, TL_BUFFER_TYPE_UINT1, lenght, elements);

	TLDRE;
}

TLOVERLOAD void tl_graphics_geometry_elements(TLGeometry* geometry, u32 lenght, u16* elements) {
    TLDPUSH;

    tl_graphics_geometry_elements(geometry, TL_BUFFER_TYPE_USHORT1, lenght, elements);

	TLDRE;
}

void tl_graphics_geometry_vertices(TLGeometry* geometry, u32 lenght, f32* vertices) {
    TLDPUSH;
    
    if (geometry == NULL) TLDWRE("TLGeometry is NULL");
    u32 bytes = tl_parse_buffer_bytes(TL_BUFFER_TYPE_FLOAT1);
    u32 size = lenght * bytes;
    
    // Expand the buffer to acomodate the vertices
    if (geometry->vbo_length != lenght) {
        TLTRACE("TLGeometry[vao %d] Expanding VBO from %lu to %lu bytes", geometry->vao, geometry->vbo_length * bytes, size);
        if (geometry->vbo != GL_NONE) {  glDeleteBuffers(1, &geometry->vbo);  }
        glCreateBuffers(1, &geometry->vbo);
        glNamedBufferStorage(geometry->vbo, size, NULL, GL_DYNAMIC_STORAGE_BIT);
        glVertexArrayVertexBuffer(geometry->vao, 0, geometry->vbo, 0, geometry->vbo_stride);
        geometry->vbo_length = lenght;
    }

    // Push the data into the GPU
    glNamedBufferSubData(geometry->vbo, 0, size, vertices);

	TLDRE;
}

void tl_graphics_geometry_bind(TLGeometry* geometry) {
    TLDPUSH;
    
    if (geometry == NULL) TLDWRE("TLGeometry is NULL");
    if (state.vao == geometry->vao) TLDRE;
    state.vao = geometry->vao;
    glBindVertexArray(state.vao);

	TLDRE;
}

void tl_graphics_geometry_destroy(TLGeometry* geometry) {
    TLDPUSH;
    
    if (geometry == NULL) TLDWRE("TLGeometry is NULL");
    if (geometry->ebo != GL_NONE) glDeleteBuffers(1, &geometry->ebo);
    if (geometry->vbo != GL_NONE) glDeleteBuffers(1, &geometry->vbo);
    if (geometry->vao != GL_NONE) glDeleteVertexArrays(1, &geometry->vao);
    tl_memory_free(TL_MEMORY_GRAPHICS_GEOMETRY, sizeof(TLGeometry), geometry);

	TLDRE;
}
