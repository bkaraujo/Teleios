#ifndef TELEIOS_RESOURCE
#define TELEIOS_RESOURCE

#include "teleios/types.h"
b8 tl_resource_initialize(void);

TLAudioBuffer* tl_resource_audio(const char* path);
TLShaderSource* tl_resource_shader_source(const char* path);
TLShaderProgram* tl_resource_shader_program(const char* name, u8 length, const char** path);

b8 tl_resource_terminate(void);

#endif // TELEIOS_RESOURCE