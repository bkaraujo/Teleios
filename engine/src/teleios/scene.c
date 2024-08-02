#include "teleios/teleios.h"

typedef struct {
    const char* name;
    b8(*initializer)(void);
    b8(*terminator)(void);
} TLScene;

static TLScene scenes[U8MAX] = { 0 };

#define TLCHECK(scene) if (scenes[scene].name == NULL) TLFATAL("Invalid scene")

u8 tl_scene_create(const char* name) {
    TLDPUSH;
    for (u8 i = 0 ; i < U8MAX ; ++i) {
        if (scenes[i].name != NULL) continue;
        scenes[i].name = tl_string_clone(name);
        TLDRV(i);
    }
    TLFATAL("Failed to create scene");
}

void tl_scene_set_initializer(u8 scene, b8(*initializer)(void)) {
    TLDPUSH;
    TLCHECK(scene);
    scenes[scene].initializer = initializer;
    TLDRE;
}

void tl_scene_set_terminator(u8 scene, b8(*terminator)(void)) {
    TLDPUSH;
    TLCHECK(scene);
    scenes[scene].terminator = terminator;
    TLDRE;
}

void tl_scene_destroy(u8 scene) {
    TLDPUSH;
    if (scenes[scene].name != NULL) {
        tl_string_free(scenes[scene].name);
    }
    TLDRE;
}