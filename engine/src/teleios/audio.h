#ifndef TELEIOS_AUDIO
#define TELEIOS_AUDIO

#include "teleios/types.h"

b8 tl_audio_initialize(void);

void tl_audio_destroy_buffer(TLAudioBuffer* buffer);

b8 tl_audio_terminate(void);

#endif // TELEIOS_AUDIO