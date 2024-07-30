#include "teleios/teleios.h"
#include "teleios/state.h"
#include "glad/glad.h"

// #####################################################################################################
//
//                                           H E L P E R S
//
// #####################################################################################################

typedef struct {
    u32 vao;
    u32 shader;
} TLGraphicsState;

static TLGraphicsState state = { 0 };

static u32 tl_parse_shader_stage(TLShaderStage stage) {
    switch (stage) {
        default: return U32MAX;
        case TL_SHADER_STAGE_COMPUTE: return GL_COMPUTE_SHADER;
        case TL_SHADER_STAGE_FRAGMENT: return GL_FRAGMENT_SHADER;
        case TL_SHADER_STAGE_VERTEX: return GL_VERTEX_SHADER;
    }
}

static u8 tl_parse_buffer_size(TLBufferType type) {
    switch(type) {
        default: TLFATAL("Unexpected buffer type: %d", type);

        case TL_BUFFER_TYPE_MARIX22: return 4;

        case TL_BUFFER_TYPE_MARIX32:
        case TL_BUFFER_TYPE_MARIX23: return 6;

        case TL_BUFFER_TYPE_MARIX24:
        case TL_BUFFER_TYPE_MARIX42: return 8;

        case TL_BUFFER_TYPE_MARIX33: return 9;

        case TL_BUFFER_TYPE_MARIX34:
        case TL_BUFFER_TYPE_MARIX43: return 12;

        case TL_BUFFER_TYPE_MARIX44: return 16;

        case TL_BUFFER_TYPE_BYTE1:
        case TL_BUFFER_TYPE_SHORT1:
        case TL_BUFFER_TYPE_INT1:
        case TL_BUFFER_TYPE_UBYTE1:
        case TL_BUFFER_TYPE_USHORT1:
        case TL_BUFFER_TYPE_UINT1:
        case TL_BUFFER_TYPE_FLOAT1:
        case TL_BUFFER_TYPE_DOUBLE1: return 1;

        case TL_BUFFER_TYPE_BYTE2:
        case TL_BUFFER_TYPE_SHORT2:
        case TL_BUFFER_TYPE_INT2:
        case TL_BUFFER_TYPE_UBYTE2:
        case TL_BUFFER_TYPE_USHORT2:
        case TL_BUFFER_TYPE_UINT2:
        case TL_BUFFER_TYPE_FLOAT2:
        case TL_BUFFER_TYPE_DOUBLE2: return 2;
        
        case TL_BUFFER_TYPE_BYTE3:
        case TL_BUFFER_TYPE_SHORT3:
        case TL_BUFFER_TYPE_INT3:
        case TL_BUFFER_TYPE_UBYTE3:
        case TL_BUFFER_TYPE_USHORT3:
        case TL_BUFFER_TYPE_UINT3:
        case TL_BUFFER_TYPE_FLOAT3:
        case TL_BUFFER_TYPE_DOUBLE3: return 3;
        
        case TL_BUFFER_TYPE_BYTE4:
        case TL_BUFFER_TYPE_SHORT4:
        case TL_BUFFER_TYPE_INT4:
        case TL_BUFFER_TYPE_UBYTE4:
        case TL_BUFFER_TYPE_USHORT4:
        case TL_BUFFER_TYPE_UINT4:
        case TL_BUFFER_TYPE_FLOAT4:
        case TL_BUFFER_TYPE_DOUBLE4: return 4;
    }
}

static u32 tl_parse_buffer_type(TLBufferType type) {
    switch(type) {
        default: TLFATAL("Unexpected buffer type: %d", type);
        
        case TL_BUFFER_TYPE_BYTE1:
        case TL_BUFFER_TYPE_BYTE2:
        case TL_BUFFER_TYPE_BYTE3:
        case TL_BUFFER_TYPE_BYTE4: return GL_BYTE;
        
        case TL_BUFFER_TYPE_SHORT1:
        case TL_BUFFER_TYPE_SHORT2:
        case TL_BUFFER_TYPE_SHORT3:
        case TL_BUFFER_TYPE_SHORT4: return GL_SHORT;

        case TL_BUFFER_TYPE_INT1:
        case TL_BUFFER_TYPE_INT2:
        case TL_BUFFER_TYPE_INT3:
        case TL_BUFFER_TYPE_INT4: return GL_INT;
        
        case TL_BUFFER_TYPE_UBYTE1:
        case TL_BUFFER_TYPE_UBYTE2:
        case TL_BUFFER_TYPE_UBYTE3:
        case TL_BUFFER_TYPE_UBYTE4: return GL_UNSIGNED_BYTE;
        
        case TL_BUFFER_TYPE_USHORT1:
        case TL_BUFFER_TYPE_USHORT2:
        case TL_BUFFER_TYPE_USHORT3:
        case TL_BUFFER_TYPE_USHORT4: return GL_UNSIGNED_SHORT;

        case TL_BUFFER_TYPE_UINT1:
        case TL_BUFFER_TYPE_UINT2:
        case TL_BUFFER_TYPE_UINT3:
        case TL_BUFFER_TYPE_UINT4: return GL_UNSIGNED_INT;

        case TL_BUFFER_TYPE_FLOAT1:
        case TL_BUFFER_TYPE_FLOAT2:
        case TL_BUFFER_TYPE_FLOAT3:
        case TL_BUFFER_TYPE_FLOAT4: return GL_FLOAT;
        
        case TL_BUFFER_TYPE_DOUBLE1:
        case TL_BUFFER_TYPE_DOUBLE2:
        case TL_BUFFER_TYPE_DOUBLE3:
        case TL_BUFFER_TYPE_DOUBLE4: return GL_DOUBLE;
    }
}

static u32 tl_parse_buffer_bytes(TLBufferType type) {
    switch(type) {
        default: TLFATAL("Unexpected buffer type: %d", type);
        
        case TL_BUFFER_TYPE_BYTE1: return 1 * sizeof(i8);
        case TL_BUFFER_TYPE_BYTE2: return 2 * sizeof(i8);
        case TL_BUFFER_TYPE_BYTE3: return 3 * sizeof(i8);
        case TL_BUFFER_TYPE_BYTE4: return 4 * sizeof(i8);
        
        case TL_BUFFER_TYPE_SHORT1: return 1 * sizeof(i16);
        case TL_BUFFER_TYPE_SHORT2: return 2 * sizeof(i16);
        case TL_BUFFER_TYPE_SHORT3: return 3 * sizeof(i16);
        case TL_BUFFER_TYPE_SHORT4: return 4 * sizeof(i16);

        case TL_BUFFER_TYPE_INT1: return 1 * sizeof(i32);
        case TL_BUFFER_TYPE_INT2: return 2 * sizeof(i32);
        case TL_BUFFER_TYPE_INT3: return 3 * sizeof(i32);
        case TL_BUFFER_TYPE_INT4: return 4 * sizeof(i32);
        
        case TL_BUFFER_TYPE_UBYTE1: return 1 * sizeof(u8);
        case TL_BUFFER_TYPE_UBYTE2: return 2 * sizeof(u8);
        case TL_BUFFER_TYPE_UBYTE3: return 3 * sizeof(u8);
        case TL_BUFFER_TYPE_UBYTE4: return 4 * sizeof(u8);
        
        case TL_BUFFER_TYPE_USHORT1: return 1 * sizeof(u16);
        case TL_BUFFER_TYPE_USHORT2: return 2 * sizeof(u16);
        case TL_BUFFER_TYPE_USHORT3: return 3 * sizeof(u16);
        case TL_BUFFER_TYPE_USHORT4: return 4 * sizeof(u16);

        case TL_BUFFER_TYPE_UINT1: return 1 * sizeof(u32);
        case TL_BUFFER_TYPE_UINT2: return 2 * sizeof(u32);
        case TL_BUFFER_TYPE_UINT3: return 3 * sizeof(u32);
        case TL_BUFFER_TYPE_UINT4: return 4 * sizeof(u32);

        case TL_BUFFER_TYPE_FLOAT1: return 1 * sizeof(f32);
        case TL_BUFFER_TYPE_FLOAT2: return 2 * sizeof(f32);
        case TL_BUFFER_TYPE_FLOAT3: return 3 * sizeof(f32);
        case TL_BUFFER_TYPE_FLOAT4: return 4 * sizeof(f32);
        
        case TL_BUFFER_TYPE_DOUBLE1: return 1 * sizeof(f64);
        case TL_BUFFER_TYPE_DOUBLE2: return 2 * sizeof(f64);
        case TL_BUFFER_TYPE_DOUBLE3: return 3 * sizeof(f64);
        case TL_BUFFER_TYPE_DOUBLE4: return 4 * sizeof(f64);
    }
}

static u32 tl_parse_geometry_mode(TLGeometryMode mode) {
    switch (mode) {
        case TL_GEOMETRY_MODE_TRIANGLES: return GL_TRIANGLES;
    }

    TLFATAL("Failed to parse TLGeometryMode");
}

// #####################################################################################################
//
//                                           S H A D E R
//
// #####################################################################################################

TLShaderProgram* tl_graphics_shader_create(TLShaderCreateInfo* info) {
    TLDPUSH;
    if (info == NULL) TLFATAL("TLShaderCreateInfo is NULL");
    if (info->name == NULL) TLFATAL("TLShaderCreateInfo->name is NULL");
    if (info->quantity > TL_SHADER_STAGE_MAXIMUM) TLFATAL("TLShaderCreateInfo->quantity beyond TL_SHADER_STAGE_MAXIMUM");
    if (info->sources == NULL) TLFATAL("TLShaderCreateInfo->sources is NULL");
    
    u32 stage_count[TL_SHADER_STAGE_MAXIMUM] = { 0 };
    u32 shaders[TL_SHADER_STAGE_MAXIMUM] = { 0 };

    u32 handle = glCreateProgram();
    for (u8 i = 0 ; i < info->quantity ; ++i) {
        TLShaderSource* source = info->sources[i];
        
        u32 stage = tl_parse_shader_stage(source->stage);
        if (stage == U32MAX) {
            TLWARN("Invalid shader (%s) stage", source->name);
            TLDRV(NULL);
        }
        // =================================================================
        // Ensure single script for shader stage
        // =================================================================
        if(stage_count[source->stage] > 1) {
            TLWARN("Duplicated shader (%s) stage", source->name);
            TLDRV(NULL);
        }

        stage_count[source->stage]++;
        // =================================================================
        // Compile the shader source
        // =================================================================
        u32 shader = glCreateShader(stage);
        shaders[i] = shader;

        glShaderSource(shader, 1, &source->script, NULL);
        glCompileShader(shader);

        i32 success; 
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char message[1024];
            glGetShaderInfoLog(shader, 1024, NULL, message);
            TLWARN("Failed to compile shader (%s): %s", source->name, message);

            for (u8 i = 0 ; i < info->quantity ; ++i) {
                if (shaders[i] == 0) continue;
                glDeleteShader(shaders[i]);
            }

            TLDRV(NULL);
        }

        // =================================================================
        // Attach the compiled shader to the program
        // =================================================================
        glAttachShader(handle, shader);
    }
    // =================================================================
    // Link attacked shader stages into shader program
    // =================================================================
    glLinkProgram(handle);
    i32 success;  glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if(!success) {
        char message[1024];
        glGetProgramInfoLog(handle, 1024, NULL, message);
        TLWARN("Failed to link program (%s): %s", info->name, message);
        for (u8 i = 0 ; i < info->quantity ; ++i) {
            if (shaders[i] == 0) continue;
            glDeleteShader(shaders[i]);
        }

        TLDRV(NULL);
    }
    // =================================================================
    // Delete created shader handles
    // =================================================================
    for (u8 i = 0 ; i < info->quantity ; ++i) {
        if (shaders[i] == 0) continue;
        glDeleteShader(shaders[i]);
    }
    // =================================================================
    // Build and return the API object
    // =================================================================
    TLShaderProgram* program = tl_memory_alloc(TL_MEMORY_GRAPHICS, sizeof(TLShaderProgram));
    program->handle = handle;
    program->name = info->name;

    return program;
}

void tl_graphics_shader_bind(TLShaderProgram* program) {
    TLDPUSH;
    if (program == NULL) TLDWRE("TLShaderProgram is NULL");
    if (state.shader == program->handle) TLDRE;
    
    state.shader = program->handle;
    glUseProgram(state.shader);
	TLDRE;
}

void tl_graphics_shader_uniform(TLShaderProgram* program, const char* name, TLBufferType type, void* uniform) {
    TLDPUSH;
    if (name == NULL) TLDWRE("name is NULL");
    if (uniform == NULL) TLDWRE("uniform is NULL");
    if (program == NULL) TLDWRE("TLShaderProgram is NULL");

    // TODO: avoid glGetUniformLocation repetition for the same name
    u32 location = glGetUniformLocation(program->handle, name);
    if (location == -1) TLDWRE("Uniform not found");

    switch (type) {
        default: TLFATAL("Unsupported buffer type");
        case TL_BUFFER_TYPE_FLOAT1: glUniform1fv(location, 1, uniform); break;
        case TL_BUFFER_TYPE_FLOAT2: glUniform1fv(location, 2, uniform); break;
        case TL_BUFFER_TYPE_FLOAT3: glUniform1fv(location, 3, uniform); break;
        case TL_BUFFER_TYPE_FLOAT4: glUniform1fv(location, 4, uniform); break;

        case TL_BUFFER_TYPE_INT1: glUniform1iv(location, 1, uniform); break;
        case TL_BUFFER_TYPE_INT2: glUniform1iv(location, 2, uniform); break;
        case TL_BUFFER_TYPE_INT3: glUniform1iv(location, 3, uniform); break;
        case TL_BUFFER_TYPE_INT4: glUniform1iv(location, 4, uniform); break;

        case TL_BUFFER_TYPE_MARIX22: glUniformMatrix2fv(location, 2*2, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX23: glUniformMatrix2fv(location, 2*3, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX24: glUniformMatrix2fv(location, 2*4, GL_FALSE, uniform); break;

        case TL_BUFFER_TYPE_MARIX32: glUniformMatrix3fv(location, 3*2, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX33: glUniformMatrix3fv(location, 3*3, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX34: glUniformMatrix3fv(location, 3*4, GL_FALSE, uniform); break;

        case TL_BUFFER_TYPE_MARIX42: glUniformMatrix4fv(location, 4*2, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX43: glUniformMatrix4fv(location, 4*3, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX44: glUniformMatrix4fv(location, 4*4, GL_FALSE, uniform); break;
    }

	TLDRE;
}

void tl_graphics_shader_destroy(TLShaderProgram* program) {
    TLDPUSH;
    if (program == NULL) TLDWRE("TLShaderProgram is NULL");

    glDeleteProgram(program->handle);
    tl_memory_free(TL_MEMORY_GRAPHICS, sizeof(TLShaderProgram), program);

	TLDRE;
}

// #####################################################################################################
//
//                                            G E O M E T R Y
//
// #####################################################################################################

TLGeometry* tl_graphics_geometry_create(TLGeometryCreateInfo* info) {
    TLDPUSH;
    if (info == NULL) TLDWRV("TLGeometryCreateInfo is NULL", NULL);
    if (info->buffers_length == 0) TLDWRV("TLGeometryCreateInfo->buffers_length == 0", NULL);
    if (info->buffers == NULL) TLDWRV("TLGeometryCreateInfo->buffers is NULL", NULL);

    TLGeometry* geometry = tl_memory_alloc(TL_MEMORY_GRAPHICS, sizeof(TLGeometry));
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
    tl_memory_free(TL_MEMORY_GRAPHICS, sizeof(TLGeometry), geometry);

	TLDRE;
}

// #####################################################################################################
//
//                                           R E N D E R E R
//
// #####################################################################################################

void tl_graphics_clear(void) {
    TLDPUSH;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    TLDRE;
}

void tl_graphics_draw(TLGeometry* geometry) {
    TLDPUSH;
    
    glDrawElements(
        tl_parse_geometry_mode(geometry->mode), 
        geometry->ebo_length, 
        tl_parse_buffer_type(geometry->ebo_type), 
        0
    );

	TLDRE;
}