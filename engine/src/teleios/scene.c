#include "teleios/teleios.h"

typedef struct {
    u8 handle;
    const char* name;
} TLScene;

static TLScene scenes[U8MAX] = { 0 };

u8 tl_scene_create(const char* name) {
    TLDPUSH;
    for (u8 i = 0 ; i < U8MAX ; ++i) {
        if (scenes[i].handle != TL_NONE) continue;

        scenes[i].handle = i + 1;
        scenes[i].name = name;

        TLDRV(scenes[i].handle);
    }

    TLDRV(TL_NONE);
}

void tl_scene_destroy(u8 scene) {
    TLDPUSH;
    
    scenes[scene].handle = TL_NONE;
    scenes[scene].name = NULL;
    
    TLDRE;
}