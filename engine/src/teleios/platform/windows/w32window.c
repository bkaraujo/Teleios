#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include "teleios/platform/windows/w32extern.h"
#include "teleios/platform/memory.h"
#include "teleios/platform/window.h"
#include "teleios/logger.h"
#include "teleios/diagnostic/stacktrace.h"
#include "teleios/messaging/bus.h"
#include "teleios/messaging/codes.h"

void tl_platform_window_create(TLCreateWindowInfo* info){
    TLDIAGNOSTICS_PUSH;
    // ##################################################
    // Window styles
    // ##################################################
    u32 window_ex_style = WS_EX_APPWINDOW;
    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    
    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;
    // ##################################################
    // Compute the size based on the desired content area
    // ##################################################
    RECT border_rect = { 0 };
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    u32 window_width = info->width + (border_rect.right - border_rect.left);
    u32 window_height = info->height + (border_rect.bottom - border_rect.top);
    // ##################################################
    // Centralize the window
    // ##################################################
    u32 window_x = GetSystemMetrics(SM_CXSCREEN) / 2 - window_width / 2;
    u32 window_y = GetSystemMetrics(SM_CYSCREEN) / 2 - window_height / 2;
    // ##############################################################################
    // Convert from char* to wchar_t*
    // ##############################################################################
    i32 length = MultiByteToWideChar(CP_OEMCP, MB_COMPOSITE, info->title, -1, NULL, 0);
    void* unicode = tl_platform_memory_salloc(length * sizeof(wchar_t));
    MultiByteToWideChar(CP_OEMCP, MB_COMPOSITE, info->title, -1, unicode, length);
    // ##################################################
    // Create the window
    // ##################################################
    e_hwnd = CreateWindowEx(
        window_ex_style,                // Window style extended
        TEXT("__teleios__"),            // Window class name
        unicode,                        // Window title
        window_style,                   // Window style
        window_x, window_y,             // Window position
        window_width, window_height,    // Window size
        0,                              // Parent
        0,                              // Window Menu Bar
        e_hinstance,                    //
        0                               // Window parameters
    );

    // ##################################################
    // Release unicode text
    // Break if failed to create window
    // ##################################################
    tl_platform_memory_sfree(unicode);
    if (e_hwnd == 0) TLFATAL("Window creation failed!");
    TLDIAGNOSTICS_POP;
}

void tl_platform_window_destroy() {
    TLDIAGNOSTICS_PUSH;
    if (e_hwnd == 0) { TLDIAGNOSTICS_POP;  return; }
    DestroyWindow(e_hwnd);
    TLDIAGNOSTICS_POP;
}

void tl_platform_window_show() {
    TLDIAGNOSTICS_PUSH;
    if (e_hwnd == 0) { TLDIAGNOSTICS_POP;  return; }
    ShowWindow(e_hwnd, SW_SHOW);
    TLDIAGNOSTICS_POP;
}

void tl_platform_window_hide() {
    TLDIAGNOSTICS_PUSH;
    if (e_hwnd == 0) { TLDIAGNOSTICS_POP;  return; }
    ShowWindow(e_hwnd, SW_HIDE);
    TLDIAGNOSTICS_POP;
}

void tl_platform_window_update() {
    TLDIAGNOSTICS_PUSH;
    if (e_hwnd == 0) { TLDIAGNOSTICS_POP;  return; }
    
    MSG msg = { 0 };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    TLDIAGNOSTICS_POP;
}

LRESULT CALLBACK tl_platform_window_function(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param) {
    TLDIAGNOSTICS_PUSH;
    if (hwnd != e_hwnd) {
        TLTRACE("hwnd 0x%x e_hwnd 0x%x", hwnd, e_hwnd);
        return DefWindowProc(hwnd, msg, w_param, l_param);;
    }
  
    switch (msg) {
        case WM_ERASEBKGND: {
            TLTRACE("WM_ERASEBKGND");
            TLDIAGNOSTICS_POP;
            return 1;
        }

        case WM_CLOSE: {
            TLTRACE("WM_CLOSE");
            tl_messaging_post(TL_MESSAGE_APPLICATION_QUIT, NULL);
            TLDIAGNOSTICS_POP;
            return 0;
        } break;

        case WM_DESTROY: {
            TLTRACE("WM_DESTROY");
            PostQuitMessage(0);
            TLDIAGNOSTICS_POP;
            return 0;
        }
    }

    LRESULT result = DefWindowProc(hwnd, msg, w_param, l_param);
    TLDIAGNOSTICS_POP;
    return result;
}

#endif // TLPLATFORM_WINDOWS
