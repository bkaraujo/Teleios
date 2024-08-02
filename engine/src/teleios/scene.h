#ifndef TELEIOS_SCENE
#define TELEIOS_SCENE

#include "teleios/types.h"

u8 tl_scene_create(const char* name);
void tl_scene_set_initializer(u8 scene, b8(*initializer)(void));
void tl_scene_set_terminator(u8 scene, b8(*terminator)(void));
void tl_scene_destroy(u8 scene);

#endif // TELEIOS_SCENE