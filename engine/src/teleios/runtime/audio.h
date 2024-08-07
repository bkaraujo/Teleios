#ifndef TELEIOS_AUDIO
#define TELEIOS_AUDIO

#include "teleios/runtime/types.h"

b8 tl_audio_initialize(void);

void tl_audio_buffer_destroy(TLAudioBuffer* buffer);

TLAudioSource* tl_audio_source_create(void);
void tl_audio_source_destroy(TLAudioSource* buffer);
void tl_audio_source_attach(TLAudioSource* source, TLAudioBuffer* buffer);
void tl_audio_source_play(TLAudioSource* source);
void tl_audio_source_pause(TLAudioSource* source);
void tl_audio_source_stop(TLAudioSource* source);
void tl_audio_source_rewind(TLAudioSource* source);
void tl_audio_source_set_gain(TLAudioSource* source, f32 desired);
void tl_audio_source_set_position(TLAudioSource* source, f32 x, f32 y, f32 z);

b8 tl_audio_terminate(void);

#endif // TELEIOS_AUDIO