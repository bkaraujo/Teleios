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

b8 tl_audio_terminate(void) {
    TLDIAGNOSTICS_PUSH;

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    TLDIAGNOSTICS_POP;
    return true;
}