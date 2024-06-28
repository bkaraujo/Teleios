#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include "teleios/platform/lifecycle.h"
#include "teleios/platform/windows/w32extern.h"

HINSTANCE hinstance;
HANDLE hconsole;
HANDLE heap;
LARGE_INTEGER frequency;

b8 tl_platform_initialize(void){
    hinstance = GetModuleHandle(NULL);
    hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    heap = HeapCreate(HEAP_NO_SERIALIZE, 0, 0);

    QueryPerformanceFrequency(&frequency);

    return true;
}

b8 tl_platform_terminate(void){
    HeapDestroy(heap);
    return true;
}

#endif // TLPLATFORM_WINDOWS
