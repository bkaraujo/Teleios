#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include "teleios/logger.h"
#include "teleios/platform/lifecycle.h"
#include "teleios/platform/windows/w32extern.h"

HINSTANCE e_hinstance;
HANDLE e_hconsole;
HANDLE e_heap;
LARGE_INTEGER e_frequency;
SYSTEMTIME e_unix_epoch;
HWND e_hwnd;

b8 tl_platform_initialize(void){
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
    wc.lpszClassName = TEXT("__teleios__");
    wc.hIcon = LoadIcon(e_hinstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpfnWndProc = tl_platform_window_function;

    if (RegisterClass(&wc) == 0)
        TLFATAL("Window registration failed: 0x%x", GetLastError());

    return true;
}

b8 tl_platform_terminate(void){
    HeapDestroy(e_heap);
    return true;
}

#endif // TLPLATFORM_WINDOWS
