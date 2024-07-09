#include "teleios/teleios.h"
#include "teleios/state.h"

TLAudioBuffer* tl_resource_audio(const char* path) {
    TLDIAGNOSTICS_PUSH;

    TLDIAGNOSTICS_POP;
    return NULL;
}

TLShaderSource* tl_resource_shader_source(const char* path) {
    TLDIAGNOSTICS_PUSH;

    if (path == NULL) { TLWARN("path is NULL"); TLDIAGNOSTICS_POP; return NULL; }
    const char* absolute = tl_string_join(engine_state->rootfs, path);
    TLFile* file = tl_filesystem_open(absolute);
    if (file == NULL) { TLWARN("Failed to open %s", absolute); tl_string_free(absolute); TLDIAGNOSTICS_POP; return NULL; }
    if (file->size == 0) { TLWARN("Unexpected file %s size %d", absolute, file->size); tl_string_free(absolute); tl_filesystem_close(file); TLDIAGNOSTICS_POP; return NULL; }

    tl_filesystem_string(file);
    if (file->string == NULL) { TLWARN("Failed to load content %s", path); tl_string_free(absolute); tl_filesystem_close(file); TLDIAGNOSTICS_POP; return NULL; }

    TLShaderSource* source = tl_memory_alloc(TL_MEMORY_GRAPHICS, sizeof(TLShaderSource));
    if (source == NULL) { TLWARN("Failed allocate TLShaderSource"); tl_string_free(absolute); tl_filesystem_close(file); TLDIAGNOSTICS_POP; return NULL; }

    source->name = path;
    source->stage = U32MAX;
    if (source->stage == U32MAX && tl_string_end_with(source->name, ".vert")) source->stage = TL_SHADER_STAGE_VERTEX;
    if (source->stage == U32MAX && tl_string_end_with(source->name, ".frag")) source->stage = TL_SHADER_STAGE_FRAGMENT;
    if (source->stage == U32MAX) {
        tl_string_free(absolute);
        tl_filesystem_close(file);
        tl_memory_free(TL_MEMORY_GRAPHICS, sizeof(TLShaderSource), source);
        TLDIAGNOSTICS_POP;
        return NULL;
    }

    source->size = file->size;
    source->script = tl_memory_alloc(TL_MEMORY_GRAPHICS, file->size);
    tl_memory_copy((void*)file->string, file->size, (void*)source->script);

    tl_string_free(absolute);
    tl_filesystem_close(file);

    TLDIAGNOSTICS_POP;
    return source;
}

TLShaderProgram* tl_resource_shader_program(const char* name, u8 length, const char** path) {
    TLDIAGNOSTICS_PUSH;
    if (length == 0) { TLWARN("Invalid array length"); return NULL; }

    TLShaderSource** sources = tl_memory_alloc(TL_MEMORY_GRAPHICS, length * sizeof(TLShaderSource));
    for (u8 i = 0; i < length ; ++i) {
        sources[i] = tl_resource_shader_source(path[i]);
        if (sources[i] == NULL) {
            tl_memory_free(TL_MEMORY_GRAPHICS, length * sizeof(TLShaderSource), sources);
            TLWARN("Failed to read shader source %d", i);
            TLDIAGNOSTICS_POP;
            return NULL;
        }
    }

    TLShaderCreateInfo shader_spec = { 0 };
    shader_spec.name = name;
    shader_spec.quantity = length;
    shader_spec.sources = sources;

    TLShaderProgram* shader = tl_graphics_shader_create(&shader_spec);    
    for (u8 i = 0; i < length ; ++i) {
        tl_memory_free(TL_MEMORY_GRAPHICS, sources[i]->size, (void*)sources[i]->script);
        tl_memory_free(TL_MEMORY_GRAPHICS, sizeof(TLShaderSource), (void*)sources[i]);
    }
    
    tl_memory_free(TL_MEMORY_GRAPHICS, length * sizeof(TLShaderSource), sources);
    if (shader == NULL) { TLERROR("Failed to create shader program");  TLDIAGNOSTICS_POP; return NULL; }

    TLDIAGNOSTICS_POP;
    return shader;
}
