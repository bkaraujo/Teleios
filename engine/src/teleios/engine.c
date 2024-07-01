#include "teleios/engine.h"
#include "teleios/logger.h"
#include "teleios/platform/lifecycle.h"
#include "teleios/platform/time.h"
#include "teleios/memory/lifecycle.h"
#include "teleios/diagnostic/lifecycle.h"
#include "teleios/diagnostic/stacktrace.h"
#include "teleios/container.h"
#include "teleios/messaging/lifecycle.h"
#include "teleios/platform/window.h"

#include "teleios/messaging/bus.h"
#include "teleios/messaging/codes.h"

static b8 running = true;

static TLMessageChain tl_engine_messaging(const u16 code, const TLMessage* message) {
    TLDIAGNOSTICS_PUSH;

    if (code == TL_MESSAGE_APPLICATION_QUIT) {
        running = false;
    }

    TLDIAGNOSTICS_POP;
    return TL_MESSAGE_AVALIABLE;
}

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

    if (!tl_messaging_initialize()) {
        TLERROR("Failed to initialize: Messaging Manager");
        TLDIAGNOSTICS_POP;
        return false;
    }

    TLCreateWindowInfo info;
    info.title = "Teleios App";
    info.width = 1024;
    info.height = 768;
    tl_platform_window_create(&info);
    
    tl_messaging_subscribe(2500, tl_engine_messaging);

    TLDIAGNOSTICS_POP;
    return true;
}

TLAPI b8 tl_engine_run(void) {
    TLDIAGNOSTICS_PUSH;
    u32 fps = 0;
    
    TLTimer timer = { 0 }; 
    tl_platform_timer_start(&timer);

    tl_platform_window_show();
    while (running) {
        fps++;
    
        tl_platform_window_update();
        tl_platform_timer_update(&timer);
        if (tl_platform_timer_seconds(&timer) >= 1.0f) {
            tl_platform_timer_start(&timer);
            TLDEBUG("FPS: %llu", fps);
            fps = 0;
        }
    }
    tl_platform_window_hide();

    TLDIAGNOSTICS_POP;
    return true;
}

TLAPI b8 tl_engine_terminate(void) {
    TLDIAGNOSTICS_PUSH;
    tl_platform_window_destroy();

    if (!tl_messaging_terminate()) {
        TLERROR("Failed to terminate: Messaging Manager");
        TLDIAGNOSTICS_POP;
        return false;
    }

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