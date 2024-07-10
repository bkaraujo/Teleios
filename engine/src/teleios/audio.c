#include "teleios/teleios.h"
#include "openal/al.h"
#include "openal/alc.h"

static ALCdevice* device;
static ALCcontext* context;

b8 tl_audio_initialize(void) {
    TLDIAGNOSTICS_PUSH;

    device = alcOpenDevice(NULL);
    if (device == NULL) TLFATAL("Failed to open audio device");

    context = alcCreateContext(device, NULL);
    if (context == NULL) TLFATAL("Failed to create audio context");

    alcMakeContextCurrent(context);

    TLDEBUG("AL_VERSION: %s", alGetString(AL_VERSION));
    TLDEBUG("AL_VENDOR: %s", alGetString(AL_VENDOR));

    TLDIAGNOSTICS_POP;
    return true;
}

void tl_audio_buffer_destroy(TLAudioBuffer* buffer) {
    TLDIAGNOSTICS_PUSH;
    if (buffer == NULL) { TLDIAGNOSTICS_POP; return; }

    if (buffer->path != NULL) tl_string_free(buffer->path);
    if (alIsBuffer(buffer->handle)) alDeleteBuffers(1, &buffer->handle);
    tl_memory_free(TL_MEMORY_AUDIO, sizeof(TLAudioBuffer), (void*) buffer);

    TLDIAGNOSTICS_POP;
}


TLAudioSource* tl_audio_source_create(void) {
    TLDIAGNOSTICS_PUSH;

    TLAudioSource* source = tl_memory_alloc(TL_MEMORY_AUDIO, sizeof(TLAudioSource));
    alGenSources(1, &source->handle);

    TLDIAGNOSTICS_POP;
    return source;
}

void tl_audio_source_attach(TLAudioSource* source, TLAudioBuffer* buffer) {
    TLDIAGNOSTICS_PUSH;
    
    source->status = TL_AUDIO_STOPED;
    source->buffer = buffer;
    TLTRACE("[AUDIO][Source %d] Attaching buffer %d", source->handle, buffer->handle);
    alSourcei(source->handle, AL_BUFFER, buffer->handle);

    TLDIAGNOSTICS_POP;
}

void tl_audio_source_play(TLAudioSource* source) {
    TLDIAGNOSTICS_PUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDIAGNOSTICS_POP; return; }
    alSourcePlay(source->handle);
    TLDIAGNOSTICS_POP;
}

void tl_audio_source_pause(TLAudioSource* source) {
    TLDIAGNOSTICS_PUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDIAGNOSTICS_POP; return; }
    alSourcePause(source->handle);
    source->status = TL_AUDIO_PAUSED;
    TLDIAGNOSTICS_POP;
}

void tl_audio_source_stop(TLAudioSource* source) {
    TLDIAGNOSTICS_PUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDIAGNOSTICS_POP; return; }
    alSourceStop(source->handle);
    source->status = TL_AUDIO_STOPED;
    TLDIAGNOSTICS_POP;
}

void tl_audio_source_rewind(TLAudioSource* source) {
    TLDIAGNOSTICS_PUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDIAGNOSTICS_POP; return; }
    alSourceRewind(source->handle);
    TLDIAGNOSTICS_POP;
}

void tl_audio_source_set_gain(TLAudioSource* source, f32 desired) {
    TLDIAGNOSTICS_PUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDIAGNOSTICS_POP; return; }
    alSourcef(source->handle, AL_GAIN, desired);
    TLDIAGNOSTICS_POP;
}

void tl_audio_source_set_position(TLAudioSource* source, f32 x, f32 y, f32 z) {
    TLDIAGNOSTICS_PUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDIAGNOSTICS_POP; return; }
    alSource3f(source->handle, AL_POSITION, x, y, z);
    TLDIAGNOSTICS_POP;
}

void tl_audio_source_destroy(TLAudioSource* source) {
    TLDIAGNOSTICS_PUSH;
    
    if (source == NULL) { TLDIAGNOSTICS_POP; return; }
    if (alIsSource(source->handle)) alDeleteSources(1, &source->handle);
    tl_memory_free(TL_MEMORY_AUDIO, sizeof(TLAudioSource), (void*) source);

    TLDIAGNOSTICS_POP;
}

b8 tl_audio_terminate(void) {
    TLDIAGNOSTICS_PUSH;

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    TLDIAGNOSTICS_POP;
    return true;
}