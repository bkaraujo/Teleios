#ifndef TELEIOS_STATE
#define TELEIOS_STATE

#include "teleios/runtime/types.h"

typedef struct {
    b8 running;
    b8 paused;

    /** Asset lookup folder */
    const char* rootfs;
} TLEngineState;

extern TLEngineState* engine_state;

#endif // TELEIOS_STATE