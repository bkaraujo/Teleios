#include "teleios/engine.h"
#include "teleios/logger.h"
#include "teleios/platform/lifecycle.h"
#include "teleios/platform/time.h"
#include "teleios/memory/lifecycle.h"
#include "teleios/diagnostic/lifecycle.h"
#include "teleios/diagnostic/stacktrace.h"
#include "teleios/container.h"

TLAPI b8 tl_engine_initialize(void) {
    if (!tl_platform_initialize()) {
        TLERROR("Failed to initialize: Platform Abstraction");
        return false;
    }

    if (!tl_diagnostic_initialize()) {
        TLERROR("Failed to initialize: Diagnostics");
        return false;
    }
    
    TLDIAGNOSTICS_PUSH;

    if (!tl_memory_initialize()) {
        TLERROR("Failed to initialize: Memory Manager");
        TLDIAGNOSTICS_POP;
        return false;
    }

    TLDIAGNOSTICS_POP;
    return true;
}

TLAPI b8 tl_engine_run(void) {
    TLDIAGNOSTICS_PUSH;
    u32 fps = 0;
    
    TLTimer timer = { 0 }; 
    tl_platform_timer_start(&timer);

    while (true) {
        fps++;
    
        tl_platform_timer_update(&timer);
        if (tl_platform_timer_seconds(&timer) >= 1.0f) {
            tl_platform_timer_start(&timer);
            TLDEBUG("FPS: %llu", fps);
            fps = 0;
        }
    }

    TLDIAGNOSTICS_POP;
    return true;
}

TLAPI b8 tl_engine_terminate(void) {
    TLDIAGNOSTICS_PUSH;
    if (!tl_memory_terminate()) {
        TLERROR("Failed to terminate: Memory Manager");
        return false;
    }

    if (!tl_diagnostic_terminate()) {
        TLERROR("Failed to terminate: Diagnostics");
        return false;
    }

    if (!tl_platform_terminate()) {
        TLERROR("Failed to terminate: Platform Abstraction");
        return false;
    }

    return true;
}