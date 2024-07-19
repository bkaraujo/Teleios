#include "teleios/platform.h"

#ifdef TLPLATFORM_WINDOWS
#include <Windows.h>
#include <windowsx.h>
#include <malloc.h>

#include "teleios/teleios.h"
#include "teleios/messagingcodes.h"

static HINSTANCE e_hinstance;
// #####################################################################################################
//
//                                           M E M O R Y
//
// #####################################################################################################
static HANDLE e_heap;

void* tl_platform_memory_halloc(u64 size) {
    return (void*)HeapAlloc(e_heap, HEAP_ZERO_MEMORY, size);
}

void tl_platform_memory_hfree(void* block) {
    HeapFree(e_heap, HEAP_NO_SERIALIZE, block);
}

void* tl_platform_memory_salloc(u64 size) {
    return (void*)_malloca(size);
}

void tl_platform_memory_sfree(void* block) {
    _freea(block);
}

void tl_platform_memory_set(void* block, u64 size, i32 value) {
    if (size == 0) return;
    FillMemory(block, size, value);
}

void tl_platform_memory_copy(void* source, u64 size, void* target) {
    if (size == 0) return;
    CopyMemory(target, source, size);
}
// #####################################################################################################
//
//                                           C O N S O L E
//
// #####################################################################################################
static HANDLE e_hconsole;

void tl_platform_console(u8 level, const char* message) {
    static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
    // =================================================================
    // Change the console color
    // Write the message
    // Rollback the console color
    // =================================================================
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(e_hconsole, &csbi);
    SetConsoleTextAttribute(e_hconsole, levels[level]);
    WriteConsole(e_hconsole, message, (DWORD)tl_string_length(message), NULL, NULL);
    SetConsoleTextAttribute(e_hconsole, csbi.wAttributes);
}
// #####################################################################################################
//
//                                           CRYPTO
//
// #####################################################################################################
i32 tl_platform_entropy(void* buffer, i32 length) {
    BOOLEAN NTAPI SystemFunction036(PVOID, ULONG);
    return !SystemFunction036(buffer, length);
}
// #####################################################################################################
//
//                                         F I L E S Y S T E M
//
// #####################################################################################################
TLFile* tl_filesystem_open(const char* path) {
    TLDPUSH;
    if (path == NULL) { TLDPOP; return NULL; }

    WIN32_FILE_ATTRIBUTE_DATA attributes;
    if(!GetFileAttributesEx(path, GetFileExInfoStandard, &attributes)){
        DWORD dwError = GetLastError(); 
        TLERROR("Failed to GetFileAttributesEx. 0x%x", dwError);
        TLDPOP;
        return NULL;
    }

    TLFile* file = tl_memory_alloc(TL_MEMORY_FILESYSTEM, sizeof(TLFile));
    file->path = tl_string_clone(path);

    LARGE_INTEGER file_size = { 0 };
    file_size.LowPart = attributes.nFileSizeLow;
    file_size.HighPart = attributes.nFileSizeHigh;
    *((u64*)&file->size) = file_size.QuadPart;

    TLDPOP;
    return file;
}

void tl_filesystem_read(TLFile* file) {
    TLDPUSH;

    if (file == NULL) { TLWARN("TLFile is NULL"); TLDPOP; return; }
    if (file->path == NULL) { TLWARN("TLFile->path is NULL"); TLDPOP; return; }
    if (file->size == 0) { TLWARN("TLFile->size is zero"); TLDPOP; return; }

    file->handle = CreateFile(
        file->path, 
        GENERIC_READ, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, 
        NULL, 
        OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, 
        NULL
    );
    
    if (file->handle == NULL || file->handle == INVALID_HANDLE_VALUE) {
        TLWARN("Failed to open file: %s", file->path);
        file->handle = NULL;
        TLDPOP; 
        return;
    }

    DWORD dwPtr = SetFilePointer(file->handle, 0, 0, FILE_BEGIN);
    if (dwPtr == INVALID_SET_FILE_POINTER) { 
        DWORD dwError = GetLastError(); 
        TLERROR("Failed to SetFilePointer. 0x%x", dwError);
        TLDPOP;
        return;
    }

    DWORD dwBytesRead = 0;
    file->payload = tl_memory_alloc(TL_MEMORY_FILESYSTEM, file->size);
    ReadFile(file->handle, (void*)file->payload, file->size, &dwBytesRead, 0);
    if (dwBytesRead != file->size) { TLWARN("Read less bytes then expected: %s", file->path); }

    TLDPOP;
}

void tl_filesystem_close(TLFile* file) {
    TLDPUSH;

    if (file == NULL) { TLDPOP; return; }
    if (file->handle != NULL) { CloseHandle(file->handle); }
    if (file->payload != NULL) { tl_memory_free(TL_MEMORY_FILESYSTEM, file->size, (void*)file->payload); }
    tl_string_free(file->path);
    tl_memory_free(TL_MEMORY_FILESYSTEM, sizeof(TLFile), file);

    TLDPOP;
}
// #####################################################################################################
//
//                                           C H R O N O
//
// #####################################################################################################
struct TLTimer{
    u64 start;
    u64 update;
};

static LARGE_INTEGER e_frequency;
static SYSTEMTIME e_unix_epoch;


TLAPI void tl_chrono_calendar_get(TLCalendar* calendar) {
    TLDPUSH;

    if (calendar == NULL) TLDWRE("TLCalendar is NULL");
    
    SYSTEMTIME st; GetLocalTime(&st);
    *(u16*)&calendar->year = st.wYear;
    *(u8*)&calendar->month = (u8)st.wMonth;
    *(u8*)&calendar->day = (u8)st.wDay;
    *(u8*)&calendar->hour = (u8)st.wHour;
    *(u8*)&calendar->minute = (u8)st.wMinute;
    *(u8*)&calendar->seconds = (u8)st.wSecond;
    *(u16*)&calendar->millis = st.wMilliseconds;

    TLDRE;
}

TLAPI u64 tl_chrono_time_epoch_micros(void) {
    TLDPUSH;
    FILETIME ft; GetSystemTimeAsFileTime(&ft);
    u64 micros = ((u64)ft.dwHighDateTime << 32 | (u64)ft.dwLowDateTime <<  0) / 10 - 11644473600000000ULL;
    TLDRV(micros);
}

TLAPI u64 tl_chrono_time_epoch_millis(void) {
    TLDPUSH;
    u64 millis = tl_chrono_time_epoch_micros() / 1000;
    TLDRV(millis);
}

TLAPI TLTimer* tl_chrono_timer_create(void) {
    TLDPUSH;
    void* timer = tl_memory_alloc(TL_MEMORY_TIMER, sizeof(TLTimer));
    TLDRV(timer);
}

void tl_chrono_timer_destroy(TLTimer* timer) {
    TLDPUSH;
    tl_memory_free(TL_MEMORY_TIMER, sizeof(TLTimer), timer);
    TLDPOP;
}

TLAPI void tl_chrono_timer_start(TLTimer* timer) {
    TLDPUSH;
    if (timer == NULL) { TLDRE; }
    LARGE_INTEGER now; QueryPerformanceCounter(&now);
    timer->start = now.QuadPart;
    timer->update = 0;
    TLDRE;
}

TLAPI void tl_chrono_timer_update(TLTimer* timer) {
    TLDPUSH;
    if (timer == NULL) { TLDRE; }
    LARGE_INTEGER now; QueryPerformanceCounter(&now);
    timer->update = now.QuadPart;
    TLDRE;
}

TLAPI u64 tl_chrono_timer_micros(TLTimer* timer) {
    TLDPUSH;
    if (timer == NULL) { TLDRV(0); }
    u64 elapsed = timer->update - timer->start;
    u64 elaspsed = (elapsed * 1000000) / e_frequency.QuadPart; // 100-nanos to micros
    TLDRV(elaspsed);
}

TLAPI f64 tl_chrono_timer_millis(TLTimer* timer) {
    TLDPUSH;
    if (timer == NULL) { TLDRV(0.0f); }
    f64 elaspsed = tl_chrono_timer_micros(timer) / 1000.0f;
    TLDRV(elaspsed);
}

TLAPI f64 tl_chrono_timer_seconds(TLTimer* timer) {
    TLDPUSH;
    if (timer == NULL) { TLDRV(0.0f); }
    f64 elaspsed = tl_chrono_timer_millis(timer) / 1000.0f;
    TLDRV(elaspsed);
}
// #####################################################################################################
//
//                                           W I N D O W
//
// #####################################################################################################
static HWND e_hwnd;

void* tl_platform_window_handle(void) {
    return &e_hwnd;
}

void tl_platform_window_create(TLWindowCreateInfo* info) {
    TLDPUSH;
    // =================================================================
    // Window styles
    // =================================================================
    u32 window_ex_style = WS_EX_APPWINDOW;
    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;
    // =================================================================
    // Compute the size based on the desired content area
    // =================================================================
    RECT border_rect = { 0 };
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    u32 window_width = info->width + (border_rect.right - border_rect.left);
    u32 window_height = info->height + (border_rect.bottom - border_rect.top);
    // =================================================================
    // Centralize the window
    // =================================================================
    u32 window_x = GetSystemMetrics(SM_CXSCREEN) / 2 - window_width / 2;
    u32 window_y = GetSystemMetrics(SM_CYSCREEN) / 2 - window_height / 2;
    // =================================================================
    // Create the window
    // =================================================================
    e_hwnd = CreateWindowEx(
        window_ex_style,                // Window style extended
        "__teleios__",                  // Window class name
        info->title,                    // Window title
        window_style,                   // Window style
        window_x, window_y,             // Window position
        window_width, window_height,    // Window size
        0,                              // Parent
        0,                              // Window Menu Bar
        e_hinstance,                    //
        0                               // Window parameters
    );
    // =================================================================
    // Break if failed to create window
    // =================================================================
    if (e_hwnd == 0) TLFATAL("Window creation failed!");
    TLDRE;
}

void tl_platform_window_destroy() {
    TLDPUSH;
    if (e_hwnd == 0) { TLDRE; }
    DestroyWindow(e_hwnd);
    TLDRE;
}

void tl_platform_window_show() {
    TLDPUSH;
    if (e_hwnd == 0) { TLDRE; }
    ShowWindow(e_hwnd, SW_SHOW);
    TLDPOP;
}

void tl_platform_window_hide() {
    TLDPUSH;
    if (e_hwnd == 0) { TLDRE; }
    ShowWindow(e_hwnd, SW_HIDE);
    TLDRE;
}

void tl_platform_window_update() {
    TLDPUSH;
    if (e_hwnd == 0) { TLDRE; }

    MSG msg = { 0 };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        // TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    TLDRE;
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
    TLDPUSH;

    switch (msg) {
    case WM_COMPACTING: TLFATAL("System memory low");
    case WM_USERCHANGED: TLFATAL("User logged out");

    case WM_ERASEBKGND: {

        TLDPOP;
        return 1;
    }

    case WM_CLOSE: {
        tl_messaging_post(TL_MESSAGE_APPLICATION_QUIT, NULL);

        TLDPOP;
        return 0;
    } break;

    case WM_DESTROY: {
        PostQuitMessage(0);
        e_hwnd = 0;

        TLDPOP;
        return 0;
    }

    case WM_MOVE: {
        TLMessage message = { 0 };
        message.u16[0] = LOWORD(l_param);
        message.u16[1] = HIWORD(l_param);
        tl_messaging_post(TL_MESSAGE_WINDOW_MOVED, &message);

        TLDPOP;
        return 0;
    } break;

    case WM_SETFOCUS: {
        tl_messaging_post(TL_MESSAGE_WINDOW_FOCUS_GAINED, NULL);

        TLDPOP;
        return 0;
    } break;

    case WM_KILLFOCUS: {
        tl_messaging_post(TL_MESSAGE_WINDOW_FOCUS_LOST, NULL);

        TLDPOP;
        return 0;
    } break;

    case WM_SIZE: {
        TLMessage message = { 0 };
        message.u16[0] = GET_X_LPARAM(l_param);
        message.u16[1] = GET_Y_LPARAM(l_param);
        
        tl_messaging_post(TL_MESSAGE_WINDOW_RESIZED, &message);

        switch (w_param) {
            case SIZE_RESTORED: {
                if (minimized || maximized) {
                    tl_messaging_post(TL_MESSAGE_WINDOW_RESTORED, &message);
                    minimized = false;
                    maximized = false;
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

        TLDPOP;
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

        TLDPOP;
        return 0;
    } break;

    case WM_MOUSEWHEEL: {
        i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
        if (z_delta == 0) { TLDPOP; return 0; }

        TLMessage message = { 0 };
        message.i8[0] = (z_delta < 0) ? -1 : 1;
        tl_messaging_post(TL_MESSAGE_INPUT_MOUSE_WHELLED, &message);

        TLDPOP;
        return 0;
    } break;

    case WM_MOUSELEAVE: {
        tl_messaging_post(TL_MESSAGE_INPUT_MOUSE_LEFT, NULL);
        mouse_inside = false;

        TLDPOP;
        return 0;

    } break;

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN: {
        TLMessage message = { 0 };
        switch (msg) {
        case WM_LBUTTONDOWN: message.i8[0] = TL_MOUSE_LEFT; break;
        case WM_MBUTTONDOWN: message.i8[0] = TL_MOUSE_MIDDLE; break;
        case WM_RBUTTONDOWN: message.i8[0] = TL_MOUSE_RIGHT; break;
        }

        tl_messaging_post(TL_MESSAGE_INPUT_MOUSE_PRESSED, &message);

        TLDPOP;
        return 0;
    } break;

    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP: {
        TLMessage message = { 0 };
        switch (msg) {
        case WM_LBUTTONDOWN: message.i8[0] = TL_MOUSE_LEFT; break;
        case WM_MBUTTONDOWN: message.i8[0] = TL_MOUSE_MIDDLE; break;
        case WM_RBUTTONDOWN: message.i8[0] = TL_MOUSE_RIGHT; break;
        }

        tl_messaging_post(TL_MESSAGE_INPUT_MOUSE_RELEASED, &message);

        TLDPOP;
        return 0;
    } break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: {
        TLMessage message = { 0 };
        message.u16[0] = tl_platform_parse_key(w_param, l_param);
        tl_messaging_post(TL_MESSAGE_INPUT_KEY_PRESSED, &message);

        TLDPOP;
        return 0;

    }
    case WM_KEYUP:
    case WM_SYSKEYUP: {
        TLMessage message = { 0 };
        message.u16[0] = tl_platform_parse_key(w_param, l_param);
        tl_messaging_post(TL_MESSAGE_INPUT_KEY_RELEASED, &message);


        TLDPOP;
        return 0;
    }
    }

    LRESULT result = DefWindowProc(hwnd, msg, w_param, l_param);
    TLDPOP;
    return result;
}
// #####################################################################################################
//
//                                           L I F E C Y C L E
//
// #####################################################################################################
void* tl_platform_handle(void) {
    return &e_hinstance;
}

b8 tl_platform_initialize(void) {
    e_hinstance = GetModuleHandle(NULL);
    e_hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    e_heap = HeapCreate(HEAP_NO_SERIALIZE, 0, 0);

    QueryPerformanceFrequency(&e_frequency);

    e_unix_epoch.wYear = 1970;
    e_unix_epoch.wMonth = 1;
    e_unix_epoch.wDay = 1;
    e_unix_epoch.wDayOfWeek = 4;
    e_unix_epoch.wHour = 0;
    e_unix_epoch.wMilliseconds = 0;
    e_unix_epoch.wMinute = 0;
    e_unix_epoch.wSecond = 0;

    WNDCLASS wc = { 0 };
    wc.style = CS_DBLCLKS;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = e_hinstance;
    wc.lpszClassName = "__teleios__";
    wc.hIcon = LoadIcon(e_hinstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpfnWndProc = tl_platform_window_function;

    if (RegisterClass(&wc) == 0)
        TLFATAL("Window registration failed: 0x%x", GetLastError());

    return true;
}

b8 tl_platform_terminate(void) {
    HeapDestroy(e_heap);
    return true;
}
#endif // TLPLATFORM_WINDOWS
