#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include "teleios/platform/windows/w32extern.h"
#include "teleios/platform/memory.h"
#include "teleios/platform/window.h"
#include "teleios/logger.h"
#include "teleios/diagnostic/stacktrace.h"
#include "teleios/messaging/bus.h"
#include "teleios/messaging/codes.h"
#include "teleios/input/codes.h"

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

static b8 minimized = false;
static b8 maximized = false;
static b8 mouse_inside = false;

static TLInputKey tl_platform_parse_key(WPARAM w_param, LPARAM l_param) {
    TLInputKey key = (u16)w_param;

    // Check for extended scan code.
    b8 is_extended = (HIWORD(l_param) & KF_EXTENDED) == KF_EXTENDED;

    // Keypress only determines if _any_ alt/ctrl/shift key is pressed. Determine which one if so.
    if (w_param == VK_MENU) {
        key = is_extended ? TL_KEY_RALT : TL_KEY_LALT;
    }
    else if (w_param == VK_SHIFT) {
        // Annoyingly, KF_EXTENDED is not set for shift keys.
        u32 left_shift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
        u32 scancode = ((l_param & (0xFF << 16)) >> 16);
        key = scancode == left_shift ? TL_KEY_LSHIFT : TL_KEY_RSHIFT;
    }
    else if (w_param == VK_CONTROL) {
        key = is_extended ? TL_KEY_RCONTROL : TL_KEY_LCONTROL;
    }

    // HACK: This is gross windows keybind crap.
    if (key == VK_OEM_1) {
        key = TL_KEY_SEMICOLON;
    }

    return key;
}

LRESULT CALLBACK tl_platform_window_function(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param) {
    TLDIAGNOSTICS_PUSH;
  
    switch (msg) {
        case WM_COMPACTING: TLFATAL("System memory low");
        case WM_USERCHANGED: TLFATAL("User logged out");

        case WM_ERASEBKGND: {
            
            TLDIAGNOSTICS_POP;
            return 1;
        }

        case WM_CLOSE: {
            tl_messaging_post(TL_MESSAGE_APPLICATION_QUIT, NULL);

            TLDIAGNOSTICS_POP;
            return 0;
        } break;

        case WM_DESTROY: {
            PostQuitMessage(0);
            e_hwnd = 0;

            TLDIAGNOSTICS_POP;
            return 0;
        }

        case WM_MOVE: {
            TLMessage message = { 0 };
            message.u16[0] = LOWORD(l_param);
            message.u16[1] = HIWORD(l_param);
            tl_messaging_post(TL_MESSAGE_WINDOW_MOVED, &message);

            TLDIAGNOSTICS_POP;
            return 0;
        } break;

        case WM_SETFOCUS: {
            tl_messaging_post(TL_MESSAGE_WINDOW_FOCUS_GAINED, NULL);

            return 0;
        } break;

        case WM_KILLFOCUS: {
            tl_messaging_post(TL_MESSAGE_WINDOW_FOCUS_LOST, NULL);

            TLDIAGNOSTICS_POP;
            return 0;
        } break;

        case WM_SIZE: {
            TLMessage message = { 0 };
            message.u16[0] = GET_X_LPARAM(l_param);
            message.u16[1] = GET_Y_LPARAM(l_param);

            switch (w_param) {
                case SIZE_RESTORED: { 
                    if (minimized || maximized) {
                        tl_messaging_post(TL_MESSAGE_WINDOW_RESTORED, &message);
                        minimized = false; 
                        maximized = false; 
                    } else {
                        tl_messaging_post(TL_MESSAGE_WINDOW_RESIZED, &message);
                    }

                } break;
                case SIZE_MAXIMIZED: {
                    minimized = false;
                    maximized = true;
                    tl_messaging_post(TL_MESSAGE_WINDOW_MAXIMIZED, &message);
                } break;
                case SIZE_MINIMIZED: {
                    minimized = true;
                    maximized = false;
                    tl_messaging_post(TL_MESSAGE_WINDOW_MINIMIZED, &message);
                } break;
            }

            TLDIAGNOSTICS_POP;
            return 0;
        } break;

        case WM_MOUSEMOVE: {
            TLMessage message = { 0 };
            message.i32[0] = GET_X_LPARAM(l_param);
            message.i32[1] = GET_Y_LPARAM(l_param);

            if (!mouse_inside) {
                tl_messaging_post(TL_MESSAGE_INPUT_MOUSE_ENTERED, &message);

                TRACKMOUSEEVENT tme = { 0 };
                tme.cbSize = sizeof(tme);
                tme.hwndTrack = hwnd;
                tme.dwFlags = TME_LEAVE;
                tme.dwHoverTime = HOVER_DEFAULT;
                TrackMouseEvent(&tme);
                mouse_inside = true;
            }

            tl_messaging_post(TL_MESSAGE_INPUT_MOUSE_MOVED, &message);

            TLDIAGNOSTICS_POP;
            return 0;
        } break;

        case WM_MOUSEWHEEL: {
            i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            if (z_delta == 0) { TLDIAGNOSTICS_POP; return 0; }
            
            TLMessage message = { 0 };
            message.i8[0] = (z_delta < 0) ? -1 : 1;
            tl_messaging_post(TL_MESSAGE_INPUT_MOUSE_WHELLED, &message);

            TLDIAGNOSTICS_POP;
            return 0;
        } break;

        case WM_MOUSELEAVE: {
            tl_messaging_post(TL_MESSAGE_INPUT_MOUSE_LEFT, NULL);
            mouse_inside = false;

            TLDIAGNOSTICS_POP;
            return 0;

        } break;

        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN: {
            TLMessage message = { 0 };
            switch (msg) {
                case WM_LBUTTONDOWN: message.i8[0] = TL_MOUSE_BUTTON_LEFT  ; break;
                case WM_MBUTTONDOWN: message.i8[0] = TL_MOUSE_BUTTON_MIDDLE; break;
                case WM_RBUTTONDOWN: message.i8[0] = TL_MOUSE_BUTTON_RIGHT ; break;
            }
            
            tl_messaging_post(TL_MESSAGE_INPUT_MOUSE_PRESSED, &message);

            TLDIAGNOSTICS_POP;
            return 0;
        } break;

        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            TLMessage message = { 0 };
            switch (msg) {
                case WM_LBUTTONDOWN: message.i8[0] = TL_MOUSE_BUTTON_LEFT  ; break;
                case WM_MBUTTONDOWN: message.i8[0] = TL_MOUSE_BUTTON_MIDDLE; break;
                case WM_RBUTTONDOWN: message.i8[0] = TL_MOUSE_BUTTON_RIGHT ; break;
            }

            tl_messaging_post(TL_MESSAGE_INPUT_MOUSE_RELEASED, &message);

            TLDIAGNOSTICS_POP;
            return 0;
        } break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: {
            TLMessage message = { 0 };
            message.u16[0] = tl_platform_parse_key(w_param, l_param);
            tl_messaging_post(TL_MESSAGE_INPUT_KEY_PRESSED, &message);

            TLDIAGNOSTICS_POP;
            return 0;

        }
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            TLMessage message = { 0 };
            message.u16[0] = tl_platform_parse_key(w_param, l_param);
            tl_messaging_post(TL_MESSAGE_INPUT_KEY_RELEASED, &message);


            TLDIAGNOSTICS_POP;
            return 0;
        }
    }

    LRESULT result = DefWindowProc(hwnd, msg, w_param, l_param);
    TLDIAGNOSTICS_POP;
    return result;
}

#endif // TLPLATFORM_WINDOWS
