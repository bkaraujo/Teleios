#include "teleios/teleios.h"
#include "teleios/state.h"
#include "openal/al.h"
#include "openal/alc.h"
#include "stb/vorbis.h"

TLAudioBuffer* tl_resource_audio(const char* path) {
    TLDPUSH;

    if (path == NULL) { TLWARN("path is NULL"); TLDPOP; return NULL; }
    const char* absolute = tl_string_join(engine_state->rootfs, path);
    
    TLFile* file = tl_filesystem_open(absolute);
    if (file == NULL) { TLWARN("Failed to open %s", absolute); tl_string_free(absolute); TLDPOP; return NULL; }
    if (file->size == 0) { TLWARN("Unexpected file %s size %d", absolute, file->size); tl_string_free(absolute); tl_filesystem_close(file); TLDPOP; return NULL; }
    tl_string_free(absolute); absolute = NULL;
    
    TLAudioBuffer* buffer = tl_memory_alloc(TL_MEMORY_AUDIO, sizeof(TLAudioBuffer));
    if (buffer == NULL) { TLWARN("Failed to allocate TLAudioBuffer"); tl_filesystem_close(file); TLDPOP; return NULL; }
    alGenBuffers(1, &buffer->handle);
    buffer->path = tl_string_clone(file->path);

    i16* vorbis;
    buffer->size = stb_vorbis_decode_filename(file->path, &buffer->channels, &buffer->sample_rate, &vorbis);
    alBufferData(buffer->handle, buffer->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, vorbis, buffer->size, buffer->sample_rate);
    free(vorbis); vorbis = NULL;

    tl_filesystem_close(file); file = NULL;

    i32 al_error = alGetError();
    if (al_error != AL_NO_ERROR) {
        tl_audio_buffer_destroy(buffer);
        buffer = NULL;
        
        TLERROR("Failed upload audio to buffer. %s", alGetString(al_error));
    }

    TLDPOP;
    return buffer;
}

TLShaderSource* tl_resource_shader_source(const char* path) {
    TLDPUSH;

    if (path == NULL) { TLWARN("path is NULL"); TLDPOP; return NULL; }
    const char* absolute = tl_string_join(engine_state->rootfs, path);
    TLFile* file = tl_filesystem_open(absolute);
    if (file == NULL) { TLWARN("Failed to open %s", absolute); tl_string_free(absolute); TLDPOP; return NULL; }
    if (file->size == 0) { TLWARN("Unexpected file %s size %d", absolute, file->size); tl_string_free(absolute); tl_filesystem_close(file); TLDPOP; return NULL; }
    tl_string_free(absolute); absolute = NULL;

    tl_filesystem_read(file);
    if (file->payload == NULL) { TLWARN("Failed to load content %s", path); tl_filesystem_close(file); TLDPOP; return NULL; }

    TLShaderSource* source = tl_memory_alloc(TL_MEMORY_GRAPHICS, sizeof(TLShaderSource));
    if (source == NULL) { TLWARN("Failed allocate TLShaderSource"); tl_filesystem_close(file); TLDPOP; return NULL; }

    source->name = path;
    source->stage = U32MAX;
    if (source->stage == U32MAX && tl_string_end_with(source->name, ".vert")) source->stage = TL_SHADER_STAGE_VERTEX;
    if (source->stage == U32MAX && tl_string_end_with(source->name, ".frag")) source->stage = TL_SHADER_STAGE_FRAGMENT;
    if (source->stage == U32MAX) {
        
        tl_filesystem_close(file);
        tl_memory_free(TL_MEMORY_GRAPHICS, sizeof(TLShaderSource), source);
        TLDPOP;
        return NULL;
    }

    source->size = file->size;
    source->script = tl_memory_alloc(TL_MEMORY_GRAPHICS, file->size);
    tl_memory_copy(file->payload, file->size, (void*)source->script);
    tl_filesystem_close(file);

    TLDPOP;
    return source;
}

TLShaderProgram* tl_resource_shader_program(const char* name, u8 length, const char** path) {
    TLDPUSH;
    if (length == 0) { TLWARN("Invalid array length"); return NULL; }

    TLShaderSource** sources = tl_memory_alloc(TL_MEMORY_GRAPHICS, length * sizeof(TLShaderSource));
    for (u8 i = 0; i < length ; ++i) {
        sources[i] = tl_resource_shader_source(path[i]);
        if (sources[i] == NULL) {
            tl_memory_free(TL_MEMORY_GRAPHICS, length * sizeof(TLShaderSource), sources);
            TLWARN("Failed to read shader source %d", i);
            TLDPOP;
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
    if (shader == NULL) { TLERROR("Failed to create shader program");  TLDPOP; return NULL; }

    TLDPOP;
    return shader;
}
