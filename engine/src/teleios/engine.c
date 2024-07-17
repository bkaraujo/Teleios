#include "teleios/engine.h"
#include "teleios/teleios.h"
#include "teleios/messagingcodes.h"
#include "teleios/state.h"
#include "teleios/platform.h"

TLEngineState* engine_state;
static u64 frame_overflow = 0;
static u64 frame_counter = -1;

static TLMessageChain tl_engine_messaging(const u16 code, const TLMessage* message) {
    TLDPUSH;

    switch (code) {
        case TL_MESSAGE_APPLICATION_PAUSE : engine_state->paused = true; break;
        case TL_MESSAGE_APPLICATION_RESUME: engine_state->paused = false; break;
        case TL_MESSAGE_APPLICATION_QUIT  : engine_state->running = false; break;
    }

    TLDPOP;
    return TL_MESSAGE_AVALIABLE;
}

TLAPI b8 tl_engine_pre_initialize(void) {
    if (!tl_platform_initialize()) { TLERROR("Failed to initialize: Platform Abstraction"); return false; }
    if (!tl_diagnostic_initialize()) { TLERROR("Failed to initialize: Diagnostics"); return false; }

    engine_state = tl_platform_memory_halloc(sizeof(TLEngineState));
    engine_state->running = false;
    engine_state->paused = false;

    return true;
}

TLAPI b8 tl_engine_initialize(void) {
    TLDPUSH;

    if (!tl_memory_initialize()) { TLERROR("Failed to initialize: Memory Manager"); TLDPOP; return false; }
    if (!tl_messaging_initialize()) { TLERROR("Failed to initialize: Messaging Manager"); TLDPOP; return false; }

    tl_messaging_subscribe(TL_MESSAGE_ALL_KNOWN, tl_engine_messaging);

    if (!tl_input_initialize()) { TLERROR("Failed to initialize: Input Manager"); TLDPOP; return false; }
    if (!tl_audio_initialize()) { TLERROR("Failed to initialize: Audio Manager"); TLDPOP; return false; }
    if (!tl_ecs_initialize()) { TLERROR("Failed to initialize: ECS System"); TLDPOP; return false; }

    TLDPOP;
    return true;
}

TLAPI b8 tl_engine_configure(TLAppSpecification* specification) {
    TLDPUSH;
    
    engine_state->rootfs = tl_memory_alloc(TL_MEMORY_RESOURCE, tl_string_length(specification->rootfs));
    tl_memory_copy((void*)specification->rootfs, tl_string_length(specification->rootfs), (void*)engine_state->rootfs);

    tl_platform_window_create(&specification->window);
    if (!tl_graphics_initialize(&specification->graphics)) { TLERROR("Failed to initialize: Graphics Manager"); TLDPOP; return false; }

    TLDPOP;
    return true;
}

TLAPI b8 tl_engine_run(void) {
    TLDPUSH;
    u32 fps = 0;
    
    TLUlid* entity;
    {
        entity = tl_ecs_entity_create();
        tl_ecs_entity_attach(entity, TLNameComponentID);
        TLNameComponent* nc = (TLNameComponent*) tl_ecs_entity_component(entity, TLNameComponentID);
        nc->name = "My Component";
    } 
    
    TLTimer timer = { 0 }; 
    tl_chrono_timer_start(&timer);
    
    {
        TLNameComponent* nc = (TLNameComponent*) tl_ecs_entity_component(entity, TLNameComponentID);
        TLINFO("TLNameComponent->name = %s", nc->name);
    }

    TLShaderProgram* shader = NULL;
    {
        const char* paths[] = { "/shader/hello.vert", "/shader/hello.frag" };
        shader = tl_resource_shader_program("hello-triangle", 2, paths);
        if (shader == NULL) { TLERROR("Failed to create TLShaderProgram"); TLDPOP; return false; }
    }

    TLGeometry* geometry = NULL;
    {
        TLGeometryBuffer gbuffer = { 0 };
        gbuffer.name = "aPos";
        gbuffer.type = TL_BUFFER_TYPE_FLOAT3;

        TLGeometryCreateInfo gspec = { 0 };
        gspec.mode = TL_GEOMETRY_MODE_TRIANGLES;
        gspec.buffers_length = 1;
        gspec.buffers = &gbuffer;

        geometry = tl_graphics_geometry_create(&gspec);
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
    }

    // Pre update so the window dont blink upon the first update    
    tl_graphics_update();
    tl_platform_window_update();
    tl_platform_window_show();

    engine_state->running = true;
    while (engine_state->running) {
        frame_counter++;
        if (frame_counter == 0) { frame_overflow++; }

        if (!engine_state->paused) {
            fps++;
            if (tl_input_key_released(TL_KEY_ESCAPE)) {
                engine_state->running = false;
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
            TLDEBUG("FPS: %u", fps);
            fps = 0;
        }
    }

    tl_platform_window_hide();
    tl_graphics_shader_destroy(shader);
    tl_graphics_geometry_destroy(geometry);
    
    tl_ecs_entity_destroy(entity);

    TLDPOP;
    return true;
}

TLAPI b8 tl_engine_terminate(void) {
    TLDPUSH;

    tl_memory_free(TL_MEMORY_RESOURCE, tl_string_length(engine_state->rootfs), (void*) engine_state->rootfs);

    if (!tl_graphics_terminate()) { TLERROR("Failed to terminate: Graphics Manager"); TLDPOP; return false; }

    tl_platform_window_destroy();

    if (!tl_ecs_terminate()) { TLERROR("Failed to terminate: ECS System"); TLDPOP; return false; }
    if (!tl_audio_terminate()) { TLERROR("Failed to terminate: Audio Manager"); TLDPOP; return false; }
    if (!tl_input_terminate()) { TLERROR("Failed to terminate: Input Manager"); TLDPOP; return false; }
    if (!tl_messaging_terminate()) { TLERROR("Failed to terminate: Messaging Manager"); TLDPOP; return false; }
    if (!tl_memory_terminate()) { TLERROR("Failed to terminate: Memory Manager"); TLDPOP; return false; }
    if (!tl_diagnostic_terminate()) { TLERROR("Failed to terminate: Diagnostics"); return false; }
    if (!tl_platform_terminate()) { TLERROR("Failed to terminate: Platform Abstraction"); return false; }

    return true;
}