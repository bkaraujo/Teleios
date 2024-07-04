#include "teleios/graphics.h"
#ifdef TLPLATFORM_WINDOWS
#include <Windows.h>

#include "teleios/platform.h"
#include "teleios/diagnostic.h"
#include "teleios/logger.h"
#include "teleios/memory.h"
#include "teleios/container.h"
#include "teleios/messaging.h"
#include "glad/glad.h"
#include "glad/wgl.h"

// #####################################################################################################
//
//                                           S H A D E R
//
// #####################################################################################################
static u32 t_shader_parse_stage(TLShaderStage stage) {
    TLDIAGNOSTICS_PUSH;
    
    u32 result;
    switch (stage) {
        default: result = U32MAX; break;
        case TL_SHADER_STAGE_COMPUTE: result = GL_COMPUTE_SHADER; break;
        case TL_SHADER_STAGE_FRAGMENT: result = GL_FRAGMENT_SHADER; break;
        case TL_SHADER_STAGE_VERTEX: result = GL_VERTEX_SHADER; break;
    }

    TLDIAGNOSTICS_POP;
    return result;
}

TLShaderProgram* tl_graphics_shader_create(TLShaderSpecification* specification) {
    TLDIAGNOSTICS_PUSH;
    if (specification == NULL) TLFATAL("TLShaderSpecification is NULL");
    if (specification->name == NULL) TLFATAL("TLShaderSpecification->name is NULL");
    if (specification->quantity > TL_SHADER_STAGE_MAXIMUM) TLFATAL("TLShaderSpecification->quantity beyond TL_SHADER_STAGE_MAXIMUM");
    if (specification->sources == NULL) TLFATAL("TLShaderSpecification->sources is NULL");
    
    u32 stage_count[TL_SHADER_STAGE_MAXIMUM] = { 0 };
    u32 shaders[TL_SHADER_STAGE_MAXIMUM] = { 0 };

    u32 handle = glCreateProgram();
    for (u8 i = 0 ; i < specification->quantity ; ++i) {
        TLShaderSource source = specification->sources[i];
        
        u32 stage = t_shader_parse_stage(source.stage);
        if (stage == U32MAX) {
            TLWARN("Invalid shader (%s) stage", source.name);
            TLDIAGNOSTICS_POP;
            return NULL;
        }
        // ##############################################
        // Ensure single script for shader stage
        // ##############################################
        if(stage_count[source.stage] > 1) {
            TLWARN("Duplicated shader (%s) stage", source.name);
            TLDIAGNOSTICS_POP;
            return NULL;
        }

        stage_count[source.stage]++;
        // ##############################################
        // Compile the shader source
        // ##############################################
        u32 shader = glCreateShader(stage);
        shaders[i] = shader;

        glShaderSource(shader, 1, &source.script, NULL);
        glCompileShader(shader);

        i32 success; 
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char message[1024];
            glGetShaderInfoLog(shader, 1024, NULL, message);
            TLWARN("Failed to compile shader (%s): %s", source.name, message);

            for (u8 i = 0 ; i < specification->quantity ; ++i) {
                if (shaders[i] == 0) continue;
                glDeleteShader(shaders[i]);
            }

            TLDIAGNOSTICS_POP;
            return NULL;
        }

        // ##############################################
        // Attach the compiled shader to the program
        // ###############################################
        glAttachShader(handle, shader);
    }
    // ##############################################
    // Link attacked shader stages into shader program
    // ##############################################
    glLinkProgram(handle);
    i32 success;  glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if(!success) {
        char message[1024];
        glGetProgramInfoLog(handle, 1024, NULL, message);
        TLWARN("Failed to link program (%s): %s", specification->name, message);
        for (u8 i = 0 ; i < specification->quantity ; ++i) {
            if (shaders[i] == 0) continue;
            glDeleteShader(shaders[i]);
        }

        TLDIAGNOSTICS_POP;
        return NULL;
    }
    // ##############################################
    // Delete created shader handles
    // ##############################################
    for (u8 i = 0 ; i < specification->quantity ; ++i) {
        if (shaders[i] == 0) continue;
        glDeleteShader(shaders[i]);
    }
    // ##############################################
    // Build and return the API object
    // ##############################################
    TLShaderProgram* program = tl_memory_alloc(TL_MEMORY_GRAPHICS, sizeof(TLShaderProgram));
    program->handle = handle;
    program->name = specification->name;

	TLDIAGNOSTICS_POP;
    return program;
}

void tl_graphics_shader_bind(TLShaderProgram* program) {
    TLDIAGNOSTICS_PUSH;
    if (program == NULL) { TLWARN("TLShaderProgram is NULL"); TLDIAGNOSTICS_POP; return; }
    glUseProgram(program->handle);
	TLDIAGNOSTICS_POP;
}

void tl_graphics_shader_uniform(TLShaderProgram* program, const char* name, TLShaderUniformType type, void* uniform) {
    TLDIAGNOSTICS_PUSH;
    if (name == NULL) { TLWARN("name is NULL"); TLDIAGNOSTICS_POP; return; }
    if (uniform == NULL) { TLWARN("uniform is NULL"); TLDIAGNOSTICS_POP; return; }
    if (program == NULL) { TLWARN("TLShaderProgram is NULL"); TLDIAGNOSTICS_POP; return; }

    // TODO: avoid glGetUniformLocation repetition for the same name
    u32 location = glGetUniformLocation(program->handle, name);
    if (location == -1) { TLWARN("Uniform name %s not found", name); TLDIAGNOSTICS_POP; return; }

    switch (type) {
        case TL_SHADER_UNIFORM_FLOAT1: glUniform1fv(location, 1, uniform); break;
        case TL_SHADER_UNIFORM_FLOAT2: glUniform1fv(location, 2, uniform); break;
        case TL_SHADER_UNIFORM_FLOAT3: glUniform1fv(location, 3, uniform); break;
        case TL_SHADER_UNIFORM_FLOAT4: glUniform1fv(location, 4, uniform); break;

        case TL_SHADER_UNIFORM_INT1: glUniform1iv(location, 1, uniform); break;
        case TL_SHADER_UNIFORM_INT2: glUniform1iv(location, 2, uniform); break;
        case TL_SHADER_UNIFORM_INT3: glUniform1iv(location, 3, uniform); break;
        case TL_SHADER_UNIFORM_INT4: glUniform1iv(location, 4, uniform); break;

        case TL_SHADER_UNIFORM_MARIX22: glUniformMatrix2fv(location, 2*2, GL_FALSE, uniform); break;
        case TL_SHADER_UNIFORM_MARIX23: glUniformMatrix2fv(location, 2*3, GL_FALSE, uniform); break;
        case TL_SHADER_UNIFORM_MARIX24: glUniformMatrix2fv(location, 2*4, GL_FALSE, uniform); break;

        case TL_SHADER_UNIFORM_MARIX32: glUniformMatrix3fv(location, 3*2, GL_FALSE, uniform); break;
        case TL_SHADER_UNIFORM_MARIX33: glUniformMatrix3fv(location, 3*3, GL_FALSE, uniform); break;
        case TL_SHADER_UNIFORM_MARIX34: glUniformMatrix3fv(location, 3*4, GL_FALSE, uniform); break;

        case TL_SHADER_UNIFORM_MARIX42: glUniformMatrix4fv(location, 4*2, GL_FALSE, uniform); break;
        case TL_SHADER_UNIFORM_MARIX43: glUniformMatrix4fv(location, 4*3, GL_FALSE, uniform); break;
        case TL_SHADER_UNIFORM_MARIX44: glUniformMatrix4fv(location, 4*4, GL_FALSE, uniform); break;
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
//                                           L I F E C Y C L E
//
// #####################################################################################################
static void tl_graphics_initialize_glextentions(PIXELFORMATDESCRIPTOR* pfd) {
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

static HWND hwnd;
static HGLRC context;
static HDC hdc;

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

    int pixel_format;
    UINT num_formats;
    wglChoosePixelFormatARB(hdc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    if (!num_formats) TLFATAL("Failed to set the OpenGL 4.6 pixel format.");

    DescribePixelFormat(hdc, pixel_format, sizeof(pfd), pfd);
    if (!SetPixelFormat(hdc, pixel_format, pfd)) TLFATAL("Failed to set the OpenGL 4.6 pixel format.");

    int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    context = wglCreateContextAttribsARB(hdc, 0, attributes);
    if (!context) TLFATAL("Failed to create OpenGL 4.6 context.");

    if (!wglMakeCurrent(hdc, context)) TLFATAL("Failed to activate OpenGL 4.6 rendering context.");
	TLDIAGNOSTICS_POP;
}

b8 tl_graphics_initialize(void) {
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

	if (!gladLoadGL()) TLFATAL("Could not initialize glad(core)");
	if (!gladLoadWGL(hdc))  TLFATAL("Could not initialize glad(wgl)");

	TLDEBUG("GL_VENDOR : %s", glGetString(GL_VENDOR));
	TLDEBUG("GL_VERSION: %s", glGetString(GL_VERSION));
	
	if (wglSwapIntervalEXT != NULL) {
		wglSwapIntervalEXT(0);
	}

	glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
	
	TLDIAGNOSTICS_POP;
    return true;
}

void tl_graphics_update(void) {
    TLDIAGNOSTICS_PUSH;
	SwapBuffers(hdc);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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