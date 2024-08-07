#include "teleios/runtime/teleios.h"
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

    TLDRV(true);
}

void tl_audio_buffer_destroy(TLAudioBuffer* buffer) {
    TLDPUSH;
    if (buffer == NULL) TLDRE;

    if (buffer->path != NULL) tl_string_free(buffer->path);
    if (alIsBuffer(buffer->handle)) alDeleteBuffers(1, &buffer->handle);
    tl_memory_free((void*) buffer);

    TLDRE;
}


TLAudioSource* tl_audio_source_create(void) {
    TLDPUSH;

    TLAudioSource* source = tl_memory_alloc(TL_MEMORY_AUDIO, sizeof(TLAudioSource));
    alGenSources(1, &source->handle);

    TLDRV(source);
}

void tl_audio_source_attach(TLAudioSource* source, TLAudioBuffer* buffer) {
    TLDPUSH;
    
    source->status = TL_AUDIO_STOPED;
    source->buffer = buffer;
    TLTRACE("[AUDIO][Source %d] Attaching buffer %d", source->handle, buffer->handle);
    alSourcei(source->handle, AL_BUFFER, buffer->handle);

    TLDRE;
}

void tl_audio_source_play(TLAudioSource* source) {
    TLDPUSH;
    
    if (source == NULL) TLDWRE("TLAudioSource is NULL");
    alSourcePlay(source->handle);

    TLDRE;
}

void tl_audio_source_pause(TLAudioSource* source) {
    TLDPUSH;

    if (source == NULL) TLDWRE("TLAudioSource is NULL");
    alSourcePause(source->handle);
    source->status = TL_AUDIO_PAUSED;

    TLDRE;
}

void tl_audio_source_stop(TLAudioSource* source) {
    TLDPUSH;

    if (source == NULL) TLDWRE("TLAudioSource is NULL");
    alSourceStop(source->handle);
    source->status = TL_AUDIO_STOPED;

    TLDRE;
}

void tl_audio_source_rewind(TLAudioSource* source) {
    TLDPUSH;

    if (source == NULL) TLDWRE("TLAudioSource is NULL");
    alSourceRewind(source->handle);

    TLDRE;
}

void tl_audio_source_set_gain(TLAudioSource* source, f32 desired) {
    TLDPUSH;

    if (source == NULL) TLDWRE("TLAudioSource is NULL");
    alSourcef(source->handle, AL_GAIN, desired);
    
    TLDRE;
}

void tl_audio_source_set_position(TLAudioSource* source, f32 x, f32 y, f32 z) {
    TLDPUSH;

    if (source == NULL) TLDWRE("TLAudioSource is NULL");
    alSource3f(source->handle, AL_POSITION, x, y, z);

    TLDRE;
}

void tl_audio_source_destroy(TLAudioSource* source) {
    TLDPUSH;
    
    if (source == NULL) TLDWRE("TLAudioSource is NULL");
    if (alIsSource(source->handle)) alDeleteSources(1, &source->handle);
    tl_memory_free((void*) source);

    TLDRE;
}

b8 tl_audio_terminate(void) {
    TLDPUSH;

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    TLDRV(true);
}