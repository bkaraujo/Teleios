#include "teleios/engine.h"
#include "teleios/logger.h"
#include "teleios/memory.h"
#include "teleios/diagnostic.h"
#include "teleios/container.h"
#include "teleios/input.h"
#include "teleios/messaging.h"
#include "teleios/messagingcodes.h"
#include "teleios/platform.h"
#include "teleios/graphics.h"

static u64 frame_overflow = 0;
static u64 frame_counter = -1;
static b8 running = true;
static b8 paused = false;

static TLMessageChain tl_engine_messaging(const u16 code, const TLMessage* message) {
    TLDIAGNOSTICS_PUSH;

    switch (code) {
        case TL_MESSAGE_APPLICATION_PAUSE : paused  = true; break;
        case TL_MESSAGE_APPLICATION_RESUME: paused  = false; break;
        case TL_MESSAGE_APPLICATION_QUIT  : running = false; break;
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

    tl_messaging_subscribe(TL_MESSAGE_ALL_KNOWN, tl_engine_messaging);

    if (!tl_input_initialize()) {
        TLERROR("Failed to initialize: Input Manager");
        TLDIAGNOSTICS_POP;
        return false;
    }

    TLCreateWindowInfo info;
    info.title = "Teleios App";
    info.width = 1024;
    info.height = 768;
    tl_platform_window_create(&info);

    if (!tl_graphics_initialize()) {
        TLERROR("Failed to initialize: Graphics Manager");
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

    tl_platform_window_show();
    while (running) {
        frame_counter++;
        if (frame_counter == 0) { frame_overflow++; }

        if (!paused) {
            fps++;
            if (tl_input_key_released(TL_KEY_ESCAPE)) {
                running = false;
            }
        }

        tl_input_update();
        tl_graphics_update();
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

    if (!tl_graphics_terminate()) {
        TLERROR("Failed to terminate: Graphics Manager");
        TLDIAGNOSTICS_POP;
        return false;
    }

    tl_platform_window_destroy();

    if (!tl_input_terminate()) {
        TLERROR("Failed to terminate: Input Manager");
        TLDIAGNOSTICS_POP;
        return false;
    }

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