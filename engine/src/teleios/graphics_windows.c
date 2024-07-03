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

static HGLRC context;
// https://gist.github.com/nickrolfe/1127313ed1dbf80254b614a721b3ee9c
b8 tl_graphics_initialize(void) {
    TLDIAGNOSTICS_PUSH;
	
	HWND hwnd = *((HWND*)tl_platform_window_handle());
	HDC hdc = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;

	i32 pixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (!pixelFormat) TLFATAL("Failed choosing pixel format!");
	if (!SetPixelFormat(hdc, pixelFormat, &pfd)) TLFATAL("Failed setting pixel format!");
    
	context = wglCreateContext(hdc);
	if (!context) TLFATAL("Failed creating OpenGL context!");
	if (!wglMakeCurrent(hdc, context)) TLFATAL("Failed setting OpenGL context!");
	if (!gladLoadGL()) TLFATAL("Could not initialize GLEW!");

	TLDIAGNOSTICS_POP;
    return true;
}

void tl_graphics_update(void) {
    TLDIAGNOSTICS_PUSH;

    TLDIAGNOSTICS_POP;
}

b8 tl_graphics_terminate(void) {
    TLDIAGNOSTICS_PUSH;

    TLDIAGNOSTICS_POP;
    return true;
}
#endif // TELEIOS_PLATFORM_WINDOWS