#include "teleios/runtime/main.h"

static u8 scene;

void tl_application_configure(TLAppSpecification* spec) {
    spec->rootfs = "X:/c/Teleios/sandbox/assets";
    
    spec->window.title = "SRzé";
    spec->window.width = 1024;
    spec->window.height = 768;

    spec->graphics.clear_color.r = 0.75f;
    spec->graphics.clear_color.g = 0.13f;
    spec->graphics.clear_color.b = 0.75f;
    spec->graphics.clear_color.a = 1.00f;
}

static TLUlid* camera;
static b8 game_scene_main_initialize(void) {
    TLUlid* camera = tl_ecs_entity_create();
    tl_ecs_entity_attach(camera, TLNameComponentID);
    TLNameComponent* name = tl_ecs_entity_component(camera, TLNameComponentID);
    name->name = "Camera";

    // TLShaderProgram* shader = NULL;
    // {
    //     const char* paths[] = { "/shader/hello.vert", "/shader/hello.frag" };
    //     shader = tl_resource_shader_program("hello-triangle", 2, paths);
    //     if (shader == NULL) { TLDERV("Failed to create TLShaderProgram", false); }
    // }

    // TLGeometry* geometry = NULL;
    // {
    //     TLGeometryBuffer gbuffer = { 0 };
    //     gbuffer.name = "aPos";
    //     gbuffer.type = TL_BUFFER_TYPE_FLOAT3;

    //     TLGeometryCreateInfo gspec = { 0 };
    //     gspec.mode = TL_GEOMETRY_MODE_TRIANGLES;
    //     gspec.buffers_length = 1;
    //     gspec.buffers = &gbuffer;

    //     geometry = tl_graphics_geometry_create(&gspec);
    //     u32 indices[] = {
    //         0, 1, 3,   // first triangle
    //         1, 2, 3    // second triangle
    //     };
    //     tl_graphics_geometry_elements(geometry, TLARRLENGTH(indices, u32), indices);
        
    //     f32 vertices[] = {
    //         0.5f,  0.5f, 0.0f,  // right top
    //         0.5f, -0.5f, 0.0f,  // right bottom 
    //         -0.5f, -0.5f, 0.0f,  // left bottom 
    //         -0.5f,  0.5f, 0.0f   // left top
    //     };
    //     tl_graphics_geometry_vertices(geometry, TLARRLENGTH(vertices, f32), vertices);
    // }
    
    return true;
}


static b8 game_scene_main_terminate(void) {
    tl_ecs_entity_destroy(camera);

    // tl_graphics_shader_destroy(shader);
    // tl_graphics_geometry_destroy(geometry);
    return true;
}

static b8 tl_application_initialize(void) {
    scene = tl_scene_create("Main");
    tl_scene_set_initializer(scene, game_scene_main_initialize);
    tl_scene_set_terminator(scene, game_scene_main_terminate);

    return true;
}

static b8 tl_application_terminate(void) {
    tl_scene_destroy(scene);

    return true;
}