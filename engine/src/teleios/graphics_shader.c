#include "teleios/teleios.h"
#include "teleios/state.h"
#include "glad/glad.h"
#include "stb/image.h"
#include "teleios/graphics_helpers.h"

u32 tl_parse_shader_stage(TLShaderStage stage) {
    switch (stage) {
        default: return U32MAX;
        case TL_SHADER_STAGE_COMPUTE: return GL_COMPUTE_SHADER;
        case TL_SHADER_STAGE_FRAGMENT: return GL_FRAGMENT_SHADER;
        case TL_SHADER_STAGE_VERTEX: return GL_VERTEX_SHADER;
    }
}

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
    TLShaderProgram* program = tl_memory_alloc(TL_MEMORY_GRAPHICS_SHADER, sizeof(TLShaderProgram));
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
    tl_memory_free(program);

	TLDRE;
}
