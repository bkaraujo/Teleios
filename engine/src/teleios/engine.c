#include "teleios/engine.h"
#include "teleios/teleios.h"
#include "teleios/messagingcodes.h"

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

TLAPI b8 tl_engine_pre_initialize(void) {
    if (!tl_platform_initialize()) { TLERROR("Failed to initialize: Platform Abstraction"); return false; }
    if (!tl_diagnostic_initialize()) { TLERROR("Failed to initialize: Diagnostics"); return false; }

    return true;
}

TLAPI b8 tl_engine_initialize(void) {
    TLDIAGNOSTICS_PUSH;

    if (!tl_memory_initialize()) { TLERROR("Failed to initialize: Memory Manager"); TLDIAGNOSTICS_POP; return false; }
    if (!tl_messaging_initialize()) { TLERROR("Failed to initialize: Messaging Manager"); TLDIAGNOSTICS_POP; return false; }

    tl_messaging_subscribe(TL_MESSAGE_ALL_KNOWN, tl_engine_messaging);

    if (!tl_input_initialize()) { TLERROR("Failed to initialize: Input Manager"); TLDIAGNOSTICS_POP; return false; }
    if (!tl_audio_initialize()) { TLERROR("Failed to initialize: Audio Manager"); TLDIAGNOSTICS_POP; return false; }

    TLDIAGNOSTICS_POP;
    return true;
}

TLAPI b8 tl_engine_configure(TLAppSpecification* specification) {
    TLDIAGNOSTICS_PUSH;
    tl_platform_window_create(&specification->window);
    if (!tl_graphics_initialize(&specification->graphics)) {
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
    tl_chrono_timer_start(&timer);

    tl_graphics_update();
    tl_platform_window_update();

    tl_platform_window_show();

    const char* paths[] = {
        "X:/c/Teleios/sandbox/assets/hello.vert",
        "X:/c/Teleios/sandbox/assets/hello.frag"
    };

    TLShaderProgram* shader = tl_resource_shader_program("hello-triangle", 2, paths);
    if (shader == NULL) TLFATAL("Failed to create shader");
    
    TLGeometryBuffer gbuffer = { 0 };
    gbuffer.name = "aPos";
    gbuffer.type = TL_BUFFER_TYPE_FLOAT3;

    TLGeometryCreateInfo gspec = { 0 };
    gspec.mode = TL_GEOMETRY_MODE_TRIANGLES;
    gspec.buffers_length = 1;
    gspec.buffers = &gbuffer;

    TLGeometry* geometry = tl_graphics_geometry_create(&gspec);
    u32 indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    tl_graphics_geometry_elements_ui(geometry, TLARRLENGTH(indices, u32), indices);
    
    f32 vertices[] = {
         0.5f,  0.5f, 0.0f,  // right top
         0.5f, -0.5f, 0.0f,  // right bottom 
        -0.5f, -0.5f, 0.0f,  // left bottom 
        -0.5f,  0.5f, 0.0f   // left top
    };
    tl_graphics_geometry_vertices(geometry, TLARRLENGTH(vertices, f32), vertices);

    while (running) {
        frame_counter++;
        if (frame_counter == 0) { frame_overflow++; }

        if (!paused) {
            fps++;
            if (tl_input_key_released(TL_KEY_ESCAPE)) {
                running = false;
            }

            tl_graphics_clear();
            tl_graphics_shader_bind(shader);
            tl_graphics_geometry_bind(geometry);
            tl_graphics_draw(geometry);
        }

        tl_input_update();
        tl_graphics_update();
        tl_platform_window_update();
        tl_chrono_timer_update(&timer);
        if (tl_chrono_timer_seconds(&timer) >= 1.0f) {
            tl_chrono_timer_start(&timer);
            TLDEBUG("FPS: %llu", fps);
            fps = 0;
        }
    }

    tl_platform_window_hide();
    tl_graphics_shader_destroy(shader);
    tl_graphics_geometry_destroy(geometry);

    TLDIAGNOSTICS_POP;
    return true;
}

TLAPI b8 tl_engine_terminate(void) {
    TLDIAGNOSTICS_PUSH;

    if (!tl_graphics_terminate()) { TLERROR("Failed to terminate: Graphics Manager"); TLDIAGNOSTICS_POP; return false; }

    tl_platform_window_destroy();

    if (!tl_audio_terminate()) { TLERROR("Failed to terminate: Audio Manager"); TLDIAGNOSTICS_POP; return false; }
    if (!tl_input_terminate()) { TLERROR("Failed to terminate: Input Manager"); TLDIAGNOSTICS_POP; return false; }
    if (!tl_messaging_terminate()) { TLERROR("Failed to terminate: Messaging Manager"); TLDIAGNOSTICS_POP; return false; }
    if (!tl_memory_terminate()) { TLERROR("Failed to terminate: Memory Manager"); TLDIAGNOSTICS_POP; return false; }
    if (!tl_diagnostic_terminate()) { TLERROR("Failed to terminate: Diagnostics"); return false; }
    if (!tl_platform_terminate()) { TLERROR("Failed to terminate: Platform Abstraction"); return false; }

    return true;
}