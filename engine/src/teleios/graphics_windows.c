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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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