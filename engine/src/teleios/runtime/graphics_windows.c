#include "teleios/runtime/graphics.h"
#ifdef TLPLATFORM_WINDOWS
#include <Windows.h>

#include "teleios/runtime/platform.h"
#include "teleios/runtime/teleios.h"
#include "teleios/runtime/messagingcodes.h"
#include "teleios/runtime/graphics_helpers.h"
#include "glad/glad.h"
#include "glad/wgl.h"

TLGraphicsState state;

// #####################################################################################################
//
//                                           L I F E C Y C L E
//
// #####################################################################################################

static HWND hwnd;
static HGLRC context;
static HDC hdc;

static void tl_graphics_initialize_glextentions(const PIXELFORMATDESCRIPTOR* pfd) {
    TLDPUSH;

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

	TLDRE;
}

static void tl_graphics_initialize_glcontext(PIXELFORMATDESCRIPTOR* pfd) {
    TLDPUSH;
	
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
#if defined(TELEIOS_BUILD_ALPHA) || defined(TELEIOS_BUILD_BETA)
        WGL_CONTEXT_FLAGS_ARB,          WGL_CONTEXT_DEBUG_BIT_ARB,
#endif // defined(TELEIOS_BUILD_ALPHA) || defined(TELEIOS_BUILD_BETA)
        0,
    };

    context = wglCreateContextAttribsARB(hdc, 0, attributes);
    if (!context) TLFATAL("Failed to create OpenGL 4.6 context.");
    if (!wglMakeCurrent(hdc, context)) TLFATAL("Failed to activate OpenGL 4.6 rendering context.");
    
	TLDRE;
}

static TLMessageChain tl_graphics_events(const u16 code, const TLMessage* message) {
    TLDPUSH;

    switch (code) {
        case TL_MESSAGE_WINDOW_RESIZED: {
            glViewport(0, 0, message->u16[0], message->u16[1]);
        } break;
    }

	TLDRV(TL_MESSAGE_CHAIN_AVALIABLE);
}

#if defined(TELEIOS_BUILD_ALPHA) || defined(TELEIOS_BUILD_BETA)
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
    TLDPUSH;

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

    TLDRE;
}
#endif // defined(TELEIOS_BUILD_ALPHA) || defined(TELEIOS_BUILD_BETA)

static void tl_graphics_initialize_glfunctions(void) {
    TLDPUSH;

	if (!gladLoadGL()) TLFATAL("Failed to initialize glad(gl)");
	if (!gladLoadWGL(hdc)) TLFATAL("Failed to initialize glad(wgl)");
	
#if defined(TELEIOS_BUILD_ALPHA) || defined(TELEIOS_BUILD_BETA)
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(tl_graphics_debug, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif // defined(TELEIOS_BUILD_ALPHA) || defined(TELEIOS_BUILD_BETA)

	TLDEBUG("GL_VENDOR : %s", glGetString(GL_VENDOR));
	TLDEBUG("GL_VERSION: %s", glGetString(GL_VERSION));
    
    TLDRE;
}

b8 tl_graphics_initialize(TLGraphicsCreateInfo* info) {
    TLDPUSH;
	
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
	
    tl_memory_zero(&state, sizeof(TLGraphicsState));

	TLDRV(true);
}

void tl_graphics_update(void) {
    TLDPUSH;
	SwapBuffers(hdc);
    TLDRE;
}

b8 tl_graphics_terminate(void) {
    TLDPUSH;

    wglMakeCurrent(hdc, 0);
    wglDeleteContext(context);
    ReleaseDC(hwnd, hdc);

    TLDRV(true);
}

#endif // TELEIOS_PLATFORM_WINDOWS