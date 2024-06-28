#include "teleios/engine.h"
#include "teleios/logger.h"
#include "teleios/platform/lifecycle.h"
#include "teleios/platform/time.h"

TLAPI b8 tl_engine_initialize(void) {
    if (!tl_platform_initialize()) {
        TLERROR("Platform failed to initialize");
        return false;
    }

    TLTRACE("tl_engine_initialize(void)");

    return true;
}

TLAPI b8 tl_engine_run(void) {
    TLTRACE("tl_engine_run(void)");

    u64 fps = 0;
    
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

    return true;
}

TLAPI b8 tl_engine_terminate(void) {
    TLTRACE("tl_engine_terminate(void)");
    if (!tl_platform_terminate()) {
        TLERROR("Platform failed to terminate");
        return false;
    }

    return true;
}