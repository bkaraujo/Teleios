#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include "teleios/platform/lifecycle.h"
#include "teleios/platform/windows/w32extern.h"

HINSTANCE hinstance;
HANDLE hconsole;
HANDLE heap;
LARGE_INTEGER frequency;
SYSTEMTIME unix_epoch;

b8 tl_platform_initialize(void){
    hinstance = GetModuleHandle(NULL);
    hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    heap = HeapCreate(HEAP_NO_SERIALIZE, 0, 0);

    QueryPerformanceFrequency(&frequency);

    unix_epoch.wYear = 1970;
    unix_epoch.wMonth = 1;
    unix_epoch.wDay = 1;
    unix_epoch.wDayOfWeek = 4;
    unix_epoch.wHour = 0;
    unix_epoch.wMilliseconds = 0;
    unix_epoch.wMinute = 0;
    unix_epoch.wSecond = 0;

    return true;
}

b8 tl_platform_terminate(void){
    HeapDestroy(heap);
    return true;
}

#endif // TLPLATFORM_WINDOWS
