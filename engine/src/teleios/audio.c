#include "teleios/teleios.h"
#include "openal/al.h"
#include "openal/alc.h"

static ALCdevice* device;
static ALCcontext* context;

b8 tl_audio_initialize(void) {
    TLDPUSH;

    device = alcOpenDevice(NULL);
    if (device == NULL) TLFATAL("Failed to open audio device");

    context = alcCreateContext(device, NULL);
    if (context == NULL) TLFATAL("Failed to create audio context");

    alcMakeContextCurrent(context);

    TLDEBUG("AL_VERSION: %s", alGetString(AL_VERSION));
    TLDEBUG("AL_VENDOR: %s", alGetString(AL_VENDOR));

    TLDPOP;
    return true;
}

void tl_audio_buffer_destroy(TLAudioBuffer* buffer) {
    TLDPUSH;
    if (buffer == NULL) { TLDPOP; return; }

    if (buffer->path != NULL) tl_string_free(buffer->path);
    if (alIsBuffer(buffer->handle)) alDeleteBuffers(1, &buffer->handle);
    tl_memory_free(TL_MEMORY_AUDIO, sizeof(TLAudioBuffer), (void*) buffer);

    TLDPOP;
}


TLAudioSource* tl_audio_source_create(void) {
    TLDPUSH;

    TLAudioSource* source = tl_memory_alloc(TL_MEMORY_AUDIO, sizeof(TLAudioSource));
    alGenSources(1, &source->handle);

    TLDPOP;
    return source;
}

void tl_audio_source_attach(TLAudioSource* source, TLAudioBuffer* buffer) {
    TLDPUSH;
    
    source->status = TL_AUDIO_STOPED;
    source->buffer = buffer;
    TLTRACE("[AUDIO][Source %d] Attaching buffer %d", source->handle, buffer->handle);
    alSourcei(source->handle, AL_BUFFER, buffer->handle);

    TLDPOP;
}

void tl_audio_source_play(TLAudioSource* source) {
    TLDPUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDPOP; return; }
    alSourcePlay(source->handle);
    TLDPOP;
}

void tl_audio_source_pause(TLAudioSource* source) {
    TLDPUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDPOP; return; }
    alSourcePause(source->handle);
    source->status = TL_AUDIO_PAUSED;
    TLDPOP;
}

void tl_audio_source_stop(TLAudioSource* source) {
    TLDPUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDPOP; return; }
    alSourceStop(source->handle);
    source->status = TL_AUDIO_STOPED;
    TLDPOP;
}

void tl_audio_source_rewind(TLAudioSource* source) {
    TLDPUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDPOP; return; }
    alSourceRewind(source->handle);
    TLDPOP;
}

void tl_audio_source_set_gain(TLAudioSource* source, f32 desired) {
    TLDPUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDPOP; return; }
    alSourcef(source->handle, AL_GAIN, desired);
    TLDPOP;
}

void tl_audio_source_set_position(TLAudioSource* source, f32 x, f32 y, f32 z) {
    TLDPUSH;
    if (source == NULL) { TLWARN("TLAudioSource is NULL"); TLDPOP; return; }
    alSource3f(source->handle, AL_POSITION, x, y, z);
    TLDPOP;
}

void tl_audio_source_destroy(TLAudioSource* source) {
    TLDPUSH;
    
    if (source == NULL) { TLDPOP; return; }
    if (alIsSource(source->handle)) alDeleteSources(1, &source->handle);
    tl_memory_free(TL_MEMORY_AUDIO, sizeof(TLAudioSource), (void*) source);

    TLDPOP;
}

b8 tl_audio_terminate(void) {
    TLDPUSH;

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    TLDPOP;
    return true;
}