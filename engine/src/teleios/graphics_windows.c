#include "teleios/graphics.h"
#ifdef TLPLATFORM_WINDOWS
#include <Windows.h>

#include "teleios/teleios.h"
#include "teleios/messagingcodes.h"
#include "glad/glad.h"
#include "glad/wgl.h"

// #####################################################################################################
//
//                                           H E L P E R S
//
// #####################################################################################################

typedef struct {
    u32 vao;
    u32 shader;
} TLGraphicsState;

static TLGraphicsState state = { 0 };

static u32 tl_parse_shader_stage(TLShaderStage stage) {
    switch (stage) {
        default: return U32MAX;
        case TL_SHADER_STAGE_COMPUTE: return GL_COMPUTE_SHADER;
        case TL_SHADER_STAGE_FRAGMENT: return GL_FRAGMENT_SHADER;
        case TL_SHADER_STAGE_VERTEX: return GL_VERTEX_SHADER;
    }
}

static u8 tl_parse_buffer_size(TLBufferType type) {
    switch(type) {
        default: TLFATAL("Unexpected buffer type: %d", type);

        case TL_BUFFER_TYPE_MARIX22: return 4;

        case TL_BUFFER_TYPE_MARIX32:
        case TL_BUFFER_TYPE_MARIX23: return 6;

        case TL_BUFFER_TYPE_MARIX24:
        case TL_BUFFER_TYPE_MARIX42: return 8;

        case TL_BUFFER_TYPE_MARIX33: return 9;

        case TL_BUFFER_TYPE_MARIX34:
        case TL_BUFFER_TYPE_MARIX43: return 12;

        case TL_BUFFER_TYPE_MARIX44: return 16;

        case TL_BUFFER_TYPE_BYTE1:
        case TL_BUFFER_TYPE_SHORT1:
        case TL_BUFFER_TYPE_INT1:
        case TL_BUFFER_TYPE_UBYTE1:
        case TL_BUFFER_TYPE_USHORT1:
        case TL_BUFFER_TYPE_UINT1:
        case TL_BUFFER_TYPE_FLOAT1:
        case TL_BUFFER_TYPE_DOUBLE1: return 1;

        case TL_BUFFER_TYPE_BYTE2:
        case TL_BUFFER_TYPE_SHORT2:
        case TL_BUFFER_TYPE_INT2:
        case TL_BUFFER_TYPE_UBYTE2:
        case TL_BUFFER_TYPE_USHORT2:
        case TL_BUFFER_TYPE_UINT2:
        case TL_BUFFER_TYPE_FLOAT2:
        case TL_BUFFER_TYPE_DOUBLE2: return 2;
        
        case TL_BUFFER_TYPE_BYTE3:
        case TL_BUFFER_TYPE_SHORT3:
        case TL_BUFFER_TYPE_INT3:
        case TL_BUFFER_TYPE_UBYTE3:
        case TL_BUFFER_TYPE_USHORT3:
        case TL_BUFFER_TYPE_UINT3:
        case TL_BUFFER_TYPE_FLOAT3:
        case TL_BUFFER_TYPE_DOUBLE3: return 3;
        
        case TL_BUFFER_TYPE_BYTE4:
        case TL_BUFFER_TYPE_SHORT4:
        case TL_BUFFER_TYPE_INT4:
        case TL_BUFFER_TYPE_UBYTE4:
        case TL_BUFFER_TYPE_USHORT4:
        case TL_BUFFER_TYPE_UINT4:
        case TL_BUFFER_TYPE_FLOAT4:
        case TL_BUFFER_TYPE_DOUBLE4: return 4;
    }
}

static u32 tl_parse_buffer_type(TLBufferType type) {
    switch(type) {
        default: TLFATAL("Unexpected buffer type: %d", type);
        
        case TL_BUFFER_TYPE_BYTE1:
        case TL_BUFFER_TYPE_BYTE2:
        case TL_BUFFER_TYPE_BYTE3:
        case TL_BUFFER_TYPE_BYTE4: return GL_BYTE;
        
        case TL_BUFFER_TYPE_SHORT1:
        case TL_BUFFER_TYPE_SHORT2:
        case TL_BUFFER_TYPE_SHORT3:
        case TL_BUFFER_TYPE_SHORT4: return GL_SHORT;

        case TL_BUFFER_TYPE_INT1:
        case TL_BUFFER_TYPE_INT2:
        case TL_BUFFER_TYPE_INT3:
        case TL_BUFFER_TYPE_INT4: return GL_INT;
        
        case TL_BUFFER_TYPE_UBYTE1:
        case TL_BUFFER_TYPE_UBYTE2:
        case TL_BUFFER_TYPE_UBYTE3:
        case TL_BUFFER_TYPE_UBYTE4: return GL_UNSIGNED_BYTE;
        
        case TL_BUFFER_TYPE_USHORT1:
        case TL_BUFFER_TYPE_USHORT2:
        case TL_BUFFER_TYPE_USHORT3:
        case TL_BUFFER_TYPE_USHORT4: return GL_UNSIGNED_SHORT;

        case TL_BUFFER_TYPE_UINT1:
        case TL_BUFFER_TYPE_UINT2:
        case TL_BUFFER_TYPE_UINT3:
        case TL_BUFFER_TYPE_UINT4: return GL_UNSIGNED_INT;

        case TL_BUFFER_TYPE_FLOAT1:
        case TL_BUFFER_TYPE_FLOAT2:
        case TL_BUFFER_TYPE_FLOAT3:
        case TL_BUFFER_TYPE_FLOAT4: return GL_FLOAT;
        
        case TL_BUFFER_TYPE_DOUBLE1:
        case TL_BUFFER_TYPE_DOUBLE2:
        case TL_BUFFER_TYPE_DOUBLE3:
        case TL_BUFFER_TYPE_DOUBLE4: return GL_DOUBLE;
    }
}

static u32 tl_parse_buffer_bytes(TLBufferType type) {
    switch(type) {
        default: TLFATAL("Unexpected buffer type: %d", type);
        
        case TL_BUFFER_TYPE_BYTE1: return 1 * sizeof(i8);
        case TL_BUFFER_TYPE_BYTE2: return 2 * sizeof(i8);
        case TL_BUFFER_TYPE_BYTE3: return 3 * sizeof(i8);
        case TL_BUFFER_TYPE_BYTE4: return 4 * sizeof(i8);
        
        case TL_BUFFER_TYPE_SHORT1: return 1 * sizeof(i16);
        case TL_BUFFER_TYPE_SHORT2: return 2 * sizeof(i16);
        case TL_BUFFER_TYPE_SHORT3: return 3 * sizeof(i16);
        case TL_BUFFER_TYPE_SHORT4: return 4 * sizeof(i16);

        case TL_BUFFER_TYPE_INT1: return 1 * sizeof(i32);
        case TL_BUFFER_TYPE_INT2: return 2 * sizeof(i32);
        case TL_BUFFER_TYPE_INT3: return 3 * sizeof(i32);
        case TL_BUFFER_TYPE_INT4: return 4 * sizeof(i32);
        
        case TL_BUFFER_TYPE_UBYTE1: return 1 * sizeof(u8);
        case TL_BUFFER_TYPE_UBYTE2: return 2 * sizeof(u8);
        case TL_BUFFER_TYPE_UBYTE3: return 3 * sizeof(u8);
        case TL_BUFFER_TYPE_UBYTE4: return 4 * sizeof(u8);
        
        case TL_BUFFER_TYPE_USHORT1: return 1 * sizeof(u16);
        case TL_BUFFER_TYPE_USHORT2: return 2 * sizeof(u16);
        case TL_BUFFER_TYPE_USHORT3: return 3 * sizeof(u16);
        case TL_BUFFER_TYPE_USHORT4: return 4 * sizeof(u16);

        case TL_BUFFER_TYPE_UINT1: return 1 * sizeof(u32);
        case TL_BUFFER_TYPE_UINT2: return 2 * sizeof(u32);
        case TL_BUFFER_TYPE_UINT3: return 3 * sizeof(u32);
        case TL_BUFFER_TYPE_UINT4: return 4 * sizeof(u32);

        case TL_BUFFER_TYPE_FLOAT1: return 1 * sizeof(f32);
        case TL_BUFFER_TYPE_FLOAT2: return 2 * sizeof(f32);
        case TL_BUFFER_TYPE_FLOAT3: return 3 * sizeof(f32);
        case TL_BUFFER_TYPE_FLOAT4: return 4 * sizeof(f32);
        
        case TL_BUFFER_TYPE_DOUBLE1: return 1 * sizeof(f64);
        case TL_BUFFER_TYPE_DOUBLE2: return 2 * sizeof(f64);
        case TL_BUFFER_TYPE_DOUBLE3: return 3 * sizeof(f64);
        case TL_BUFFER_TYPE_DOUBLE4: return 4 * sizeof(f64);
    }
}

static u32 tl_parse_geometry_mode(TLGeometryMode mode) {
    switch (mode) {
        case TL_GEOMETRY_MODE_TRIANGLES: return GL_TRIANGLES;
    }

    TLFATAL("Failed to parse TLGeometryMode");
}

// #####################################################################################################
//
//                                           S H A D E R
//
// #####################################################################################################

TLShaderProgram* tl_graphics_shader_create(TLShaderCreateInfo* info) {
    TLDIAGNOSTICS_PUSH;
    if (info == NULL) TLFATAL("TLShaderCreateInfo is NULL");
    if (info->name == NULL) TLFATAL("TLShaderCreateInfo->name is NULL");
    if (info->quantity > TL_SHADER_STAGE_MAXIMUM) TLFATAL("TLShaderCreateInfo->quantity beyond TL_SHADER_STAGE_MAXIMUM");
    if (info->sources == NULL) TLFATAL("TLShaderCreateInfo->sources is NULL");
    
    u32 stage_count[TL_SHADER_STAGE_MAXIMUM] = { 0 };
    u32 shaders[TL_SHADER_STAGE_MAXIMUM] = { 0 };

    u32 handle = glCreateProgram();
    for (u8 i = 0 ; i < info->quantity ; ++i) {
        TLShaderSource* source = info->sources[i];
        
        u32 stage = tl_parse_shader_stage(source->stage);
        if (stage == U32MAX) {
            TLWARN("Invalid shader (%s) stage", source->name);
            TLDIAGNOSTICS_POP;
            return NULL;
        }
        // =================================================================
        // Ensure single script for shader stage
        // =================================================================
        if(stage_count[source->stage] > 1) {
            TLWARN("Duplicated shader (%s) stage", source->name);
            TLDIAGNOSTICS_POP;
            return NULL;
        }

        stage_count[source->stage]++;
        // =================================================================
        // Compile the shader source
        // =================================================================
        u32 shader = glCreateShader(stage);
        shaders[i] = shader;

        glShaderSource(shader, 1, &source->script, NULL);
        glCompileShader(shader);

        i32 success; 
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char message[1024];
            glGetShaderInfoLog(shader, 1024, NULL, message);
            TLWARN("Failed to compile shader (%s): %s", source->name, message);

            for (u8 i = 0 ; i < info->quantity ; ++i) {
                if (shaders[i] == 0) continue;
                glDeleteShader(shaders[i]);
            }

            TLDIAGNOSTICS_POP;
            return NULL;
        }

        // =================================================================
        // Attach the compiled shader to the program
        // =================================================================
        glAttachShader(handle, shader);
    }
    // =================================================================
    // Link attacked shader stages into shader program
    // =================================================================
    glLinkProgram(handle);
    i32 success;  glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if(!success) {
        char message[1024];
        glGetProgramInfoLog(handle, 1024, NULL, message);
        TLWARN("Failed to link program (%s): %s", info->name, message);
        for (u8 i = 0 ; i < info->quantity ; ++i) {
            if (shaders[i] == 0) continue;
            glDeleteShader(shaders[i]);
        }

        TLDIAGNOSTICS_POP;
        return NULL;
    }
    // =================================================================
    // Delete created shader handles
    // =================================================================
    for (u8 i = 0 ; i < info->quantity ; ++i) {
        if (shaders[i] == 0) continue;
        glDeleteShader(shaders[i]);
    }
    // =================================================================
    // Build and return the API object
    // =================================================================
    TLShaderProgram* program = tl_memory_alloc(TL_MEMORY_GRAPHICS, sizeof(TLShaderProgram));
    program->handle = handle;
    program->name = info->name;

	TLDIAGNOSTICS_POP;
    return program;
}

void tl_graphics_shader_bind(TLShaderProgram* program) {
    TLDIAGNOSTICS_PUSH;
    if (program == NULL) { TLWARN("TLShaderProgram is NULL"); TLDIAGNOSTICS_POP; return; }
    if (state.shader == program->handle) { TLDIAGNOSTICS_POP; return; }
    
    state.shader = program->handle;
    glUseProgram(state.shader);
	TLDIAGNOSTICS_POP;
}

void tl_graphics_shader_uniform(TLShaderProgram* program, const char* name, TLBufferType type, void* uniform) {
    TLDIAGNOSTICS_PUSH;
    if (name == NULL) { TLWARN("name is NULL"); TLDIAGNOSTICS_POP; return; }
    if (uniform == NULL) { TLWARN("uniform is NULL"); TLDIAGNOSTICS_POP; return; }
    if (program == NULL) { TLWARN("TLShaderProgram is NULL"); TLDIAGNOSTICS_POP; return; }

    // TODO: avoid glGetUniformLocation repetition for the same name
    u32 location = glGetUniformLocation(program->handle, name);
    if (location == -1) { TLWARN("Uniform name %s not found", name); TLDIAGNOSTICS_POP; return; }

    switch (type) {
        default: TLFATAL("OPS!");
        case TL_BUFFER_TYPE_FLOAT1: glUniform1fv(location, 1, uniform); break;
        case TL_BUFFER_TYPE_FLOAT2: glUniform1fv(location, 2, uniform); break;
        case TL_BUFFER_TYPE_FLOAT3: glUniform1fv(location, 3, uniform); break;
        case TL_BUFFER_TYPE_FLOAT4: glUniform1fv(location, 4, uniform); break;

        case TL_BUFFER_TYPE_INT1: glUniform1iv(location, 1, uniform); break;
        case TL_BUFFER_TYPE_INT2: glUniform1iv(location, 2, uniform); break;
        case TL_BUFFER_TYPE_INT3: glUniform1iv(location, 3, uniform); break;
        case TL_BUFFER_TYPE_INT4: glUniform1iv(location, 4, uniform); break;

        case TL_BUFFER_TYPE_MARIX22: glUniformMatrix2fv(location, 2*2, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX23: glUniformMatrix2fv(location, 2*3, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX24: glUniformMatrix2fv(location, 2*4, GL_FALSE, uniform); break;

        case TL_BUFFER_TYPE_MARIX32: glUniformMatrix3fv(location, 3*2, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX33: glUniformMatrix3fv(location, 3*3, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX34: glUniformMatrix3fv(location, 3*4, GL_FALSE, uniform); break;

        case TL_BUFFER_TYPE_MARIX42: glUniformMatrix4fv(location, 4*2, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX43: glUniformMatrix4fv(location, 4*3, GL_FALSE, uniform); break;
        case TL_BUFFER_TYPE_MARIX44: glUniformMatrix4fv(location, 4*4, GL_FALSE, uniform); break;
    }

	TLDIAGNOSTICS_POP;
}

void tl_graphics_shader_destroy(TLShaderProgram* program) {
    TLDIAGNOSTICS_PUSH;
    if (program == NULL) { TLWARN("TLShaderProgram is NULL"); TLDIAGNOSTICS_POP; return; }

    glDeleteProgram(program->handle);
    tl_memory_free(TL_MEMORY_GRAPHICS, sizeof(TLShaderProgram), program);

	TLDIAGNOSTICS_POP;
}

// #####################################################################################################
//
//                                            G E O M E T R Y
//
// #####################################################################################################

TLGeometry* tl_graphics_geometry_create(TLGeometryCreateInfo* info) {
    TLDIAGNOSTICS_PUSH;
    if (info == NULL) { TLDIAGNOSTICS_POP; TLWARN("TLGeometryCreateInfo is NULL"); return NULL; }
    if (info->buffers_length == 0) { TLDIAGNOSTICS_POP; TLWARN("TLGeometryCreateInfo->buffers_length == 0"); return NULL; }
    if (info->buffers == NULL) { TLDIAGNOSTICS_POP; TLWARN("TLGeometryCreateInfo->buffers is NULL"); return NULL; }

    TLGeometry* geometry = tl_memory_alloc(TL_MEMORY_GRAPHICS, sizeof(TLGeometry));
    if (geometry == NULL) { TLFATAL("Failed to allocate TLGeometry"); }

    u32 offset = 0;
    glCreateVertexArrays(1, &geometry->vao);
    for (u8 i = 0; i < info->buffers_length ; ++i) {
        glEnableVertexArrayAttrib(geometry->vao, i);
        TLGeometryBuffer buffer = info->buffers[i];
        glVertexArrayAttribFormat(geometry->vao, i, tl_parse_buffer_size(buffer.type), tl_parse_buffer_type(buffer.type), GL_FALSE, offset); 
        glVertexArrayAttribBinding(geometry->vao, i, 0);
        offset += tl_parse_buffer_bytes(buffer.type);
        geometry->vbo_stride += offset;
    }

	TLDIAGNOSTICS_POP;
    return geometry;
}

static void tl_graphics_geometry_elements(TLGeometry* geometry, TLBufferType type, u32 lenght, void* elements) {
    TLDIAGNOSTICS_PUSH;
    
    if(geometry == NULL) { TLWARN("TLGeometry is NULL"); TLDIAGNOSTICS_POP; return; }
    u32 bytes = tl_parse_buffer_bytes(type);
    u32 size = lenght * bytes;

    // Expand the buffer to acomodate the elements
    if (lenght > geometry->ebo_length || geometry->ebo_type != type) {
        TLTRACE("TLGeometry[vao %d] Expanding EBO from %lu to %lu bytes", geometry->vao, geometry->ebo_length * bytes, size);
        if (geometry->ebo != GL_NONE) { glDeleteBuffers(1, &geometry->ebo); }
        glCreateBuffers(1, &geometry->ebo);
        glVertexArrayElementBuffer(geometry->vao, geometry->ebo);
        glNamedBufferStorage(geometry->ebo, size, NULL, GL_DYNAMIC_STORAGE_BIT);
        geometry->ebo_type = type;
        geometry->ebo_length = lenght;
    }

    // Push the date into the GPU
    glNamedBufferSubData(geometry->ebo, 0, size, elements);

	TLDIAGNOSTICS_POP;
}

void tl_graphics_geometry_elements_ui(TLGeometry* geometry, u32 lenght, u32* elements) {
    TLDIAGNOSTICS_PUSH;

    tl_graphics_geometry_elements(geometry, TL_BUFFER_TYPE_UINT1, lenght, elements);

	TLDIAGNOSTICS_POP;
}

void tl_graphics_geometry_elements_us(TLGeometry* geometry, u32 lenght, u16* elements) {
    TLDIAGNOSTICS_PUSH;

    tl_graphics_geometry_elements(geometry, TL_BUFFER_TYPE_USHORT1, lenght, elements);

	TLDIAGNOSTICS_POP;
}

void tl_graphics_geometry_vertices(TLGeometry* geometry, u32 lenght, f32* vertices) {
    TLDIAGNOSTICS_PUSH;
    
    if (geometry == NULL) { TLWARN("TLGeometry is NULL"); TLDIAGNOSTICS_POP; return; }
    u32 bytes = tl_parse_buffer_bytes(TL_BUFFER_TYPE_FLOAT1);
    u32 size = lenght * bytes;
    
    // Expand the buffer to acomodate the vertices
    if (geometry->vbo_length != lenght) {
        TLTRACE("TLGeometry[vao %d] Expanding VBO from %lu to %lu bytes", geometry->vao, geometry->vbo_length * bytes, size);
        if (geometry->vbo != GL_NONE) {  glDeleteBuffers(1, &geometry->vbo);  }
        glCreateBuffers(1, &geometry->vbo);
        glNamedBufferStorage(geometry->vbo, size, NULL, GL_DYNAMIC_STORAGE_BIT);
        glVertexArrayVertexBuffer(geometry->vao, 0, geometry->vbo, 0, geometry->vbo_stride);
        geometry->vbo_length = lenght;
    }

    // Push the date into the GPU
    glNamedBufferSubData(geometry->vbo, 0, size, vertices);

	TLDIAGNOSTICS_POP;
}

void tl_graphics_geometry_bind(TLGeometry* geometry) {
    TLDIAGNOSTICS_PUSH;
    
    if (geometry == NULL) { TLWARN("TLGeometry is NULL"); TLDIAGNOSTICS_POP; return; }
    if (state.vao == geometry->vao) { TLDIAGNOSTICS_POP; return; }
    state.vao = geometry->vao;
    glBindVertexArray(state.vao);

	TLDIAGNOSTICS_POP;
}

void tl_graphics_geometry_destroy(TLGeometry* geometry) {
    TLDIAGNOSTICS_PUSH;
    
    if (geometry == NULL) { TLWARN("TLGeometry is NULL"); TLDIAGNOSTICS_POP; return; }
    if (geometry->ebo != GL_NONE) glDeleteBuffers(1, &geometry->ebo);
    if (geometry->vbo != GL_NONE) glDeleteBuffers(1, &geometry->vbo);
    if (geometry->vao != GL_NONE) glDeleteVertexArrays(1, &geometry->vao);

    tl_memory_free(TL_MEMORY_GRAPHICS, sizeof(TLGeometry), geometry);

	TLDIAGNOSTICS_POP;
}

// #####################################################################################################
//
//                                           R E N D E R E R
//
// #####################################################################################################

void tl_graphics_clear(void) {
    TLDIAGNOSTICS_PUSH;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    TLDIAGNOSTICS_POP;
}

void tl_graphics_draw(TLGeometry* geometry) {
    TLDIAGNOSTICS_PUSH;
    
    glDrawElements(
        tl_parse_geometry_mode(geometry->mode), 
        geometry->ebo_length, 
        tl_parse_buffer_type(geometry->ebo_type), 
        0
    );

	TLDIAGNOSTICS_POP;
}

// #####################################################################################################
//
//                                           L I F E C Y C L E
//
// #####################################################################################################

static HWND hwnd;
static HGLRC context;
static HDC hdc;

static void tl_graphics_initialize_glextentions(const PIXELFORMATDESCRIPTOR* pfd) {
    TLDIAGNOSTICS_PUSH;

    WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.lpszClassName = "__dummygl__";

    if (!RegisterClass(&wc)) TLFATAL("Failed to register dummy OpenGL window.");

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "", 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, wc.hInstance, 0);
    if (!hwnd) TLFATAL("Failed to create dummy OpenGL window.");

    HDC hdc = GetDC(hwnd);
    int pixel_format = ChoosePixelFormat(hdc, pfd);
    if (!pixel_format) TLFATAL("Failed to find a suitable pixel format.");
    if (!SetPixelFormat(hdc, pixel_format, pfd)) TLFATAL("Failed to set the pixel format.");

    HGLRC hglrc = wglCreateContext(hdc);
    if (!hdc) TLFATAL("Failed to create a dummy OpenGL rendering context.");
    if (!wglMakeCurrent(hdc, hglrc)) TLFATAL("Failed to activate dummy OpenGL rendering context.");

    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

    wglMakeCurrent(hdc, 0);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);

	TLDIAGNOSTICS_POP;
}

static void tl_graphics_initialize_glcontext(PIXELFORMATDESCRIPTOR* pfd) {
    TLDIAGNOSTICS_PUSH;
	
	hwnd = *((HWND*)tl_platform_window_handle());
	hdc = GetDC(hwnd);
	int pixel_format_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
		WGL_SWAP_METHOD_ARB,		WGL_SWAP_EXCHANGE_ARB,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,         32,
        WGL_DEPTH_BITS_ARB,         24,
        WGL_STENCIL_BITS_ARB,       8,
		WGL_ALPHA_BITS_ARB,			8,
        0
    };

    i32 pixel_format;
    u32 num_formats;
    wglChoosePixelFormatARB(hdc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    if (!num_formats) TLFATAL("Failed to set the OpenGL 4.6 pixel format.");

    DescribePixelFormat(hdc, pixel_format, sizeof(pfd), pfd);
    if (!SetPixelFormat(hdc, pixel_format, pfd)) TLFATAL("Failed to set the OpenGL 4.6 pixel format.");

    i32 attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,  4,
        WGL_CONTEXT_MINOR_VERSION_ARB,  6,
        //WGL_CONTEXT_LAYER_PLANE_ARB, <undefined>,
        WGL_CONTEXT_PROFILE_MASK_ARB,   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#if defined(TL_BUILD_ALPHA) || defined(TL_BUILD_BETA)
        WGL_CONTEXT_FLAGS_ARB,          WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
        0,
    };

    context = wglCreateContextAttribsARB(hdc, 0, attributes);
    if (!context) TLFATAL("Failed to create OpenGL 4.6 context.");
    if (!wglMakeCurrent(hdc, context)) TLFATAL("Failed to activate OpenGL 4.6 rendering context.");
    
	TLDIAGNOSTICS_POP;
}

static TLMessageChain tl_graphics_events(const u16 code, const TLMessage* message) {
    TLDIAGNOSTICS_PUSH;

    switch (code) {
        case TL_MESSAGE_WINDOW_RESIZED: {
            glViewport(0, 0, message->u16[0], message->u16[1]);
        } break;
    }

	TLDIAGNOSTICS_POP;
    return TL_MESSAGE_AVALIABLE;
}

#if defined(TL_BUILD_ALPHA) || defined(TL_BUILD_BETA)
static const char* tl_graphics_debug_source(u32 source) {
    switch (source) {
        case GL_DEBUG_SOURCE_API:             return "API"; 
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
        case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
        case GL_DEBUG_SOURCE_OTHER:           return "Other";
    };

    return "Unknown";
}

static const char* tl_graphics_debug_type(u32 type) {
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               return "Error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behaviour"; 
        case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
        case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
        case GL_DEBUG_TYPE_MARKER:              return "Marker";
        case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
        case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
        case GL_DEBUG_TYPE_OTHER:               return "Other";
    }

    return "Unknown";
}

static const char* tl_graphics_debug_severity(u32 severity) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         return "High";
        case GL_DEBUG_SEVERITY_MEDIUM:       return "Medium";
        case GL_DEBUG_SEVERITY_LOW:          return "Low";
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
    }

    return "Unknown";
}

static void APIENTRY tl_graphics_debug(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    TLDIAGNOSTICS_PUSH;

    // ignore non-significant error/warning codes
    // if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    TLDEBUG(
        "OpenGL [%lu] :: %s's %s (%s) : %s",
        id,
        tl_graphics_debug_type(type),
        tl_graphics_debug_severity(severity),
        tl_graphics_debug_source(source),
        message
    );

    TLDIAGNOSTICS_POP;
}
#endif

static void tl_graphics_initialize_glfunctions(void) {
    TLDIAGNOSTICS_PUSH;

	if (!gladLoadGL()) TLFATAL("Failed to initialize glad(gl)");
	if (!gladLoadWGL(hdc)) TLFATAL("Failed to initialize glad(wgl)");
	
#if defined(TL_BUILD_ALPHA) || defined(TL_BUILD_BETA)
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(tl_graphics_debug, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif

	TLDEBUG("GL_VENDOR : %s", glGetString(GL_VENDOR));
	TLDEBUG("GL_VERSION: %s", glGetString(GL_VERSION));
    
    TLDIAGNOSTICS_POP;
}


b8 tl_graphics_initialize(TLGraphicsCreateInfo* info) {
    TLDIAGNOSTICS_PUSH;
	
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	
	tl_graphics_initialize_glextentions(&pfd);
	tl_graphics_initialize_glcontext(&pfd);
    tl_graphics_initialize_glfunctions();

    tl_messaging_subscribe(TL_MESSAGE_WINDOW_RESIZED, tl_graphics_events);

	if (wglSwapIntervalEXT != NULL) {
		wglSwapIntervalEXT(0);
	}

	glClearColor(
        info->clear_color.r,
        info->clear_color.g,
        info->clear_color.b,
        info->clear_color.a
    );
	
	TLDIAGNOSTICS_POP;
    return true;
}

void tl_graphics_update(void) {
    TLDIAGNOSTICS_PUSH;
	SwapBuffers(hdc);
    TLDIAGNOSTICS_POP;
}

b8 tl_graphics_terminate(void) {
    TLDIAGNOSTICS_PUSH;

    wglMakeCurrent(hdc, 0);
    wglDeleteContext(context);
    ReleaseDC(hwnd, hdc);

    TLDIAGNOSTICS_POP;
    return true;
}

#endif // TELEIOS_PLATFORM_WINDOWS