#ifndef TELEIOS_PLATFORM_WINDOWS
#define TELEIOS_PLATFORM_WINDOWS

#include "teleios/types.h"
#include <Windows.h>
#include <Windowsx.h>

extern HINSTANCE e_hinstance;
extern HANDLE e_hconsole;
extern HANDLE e_heap;
extern LARGE_INTEGER e_frequency;
extern SYSTEMTIME e_unix_epoch;
extern HWND e_hwnd;

LRESULT CALLBACK tl_platform_window_function(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);

#endif // TELEIOS_PLATFORM_WINDOWS