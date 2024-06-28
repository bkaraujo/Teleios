#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include "teleios/platform/lifecycle.h"
#include "teleios/platform/windows/w32extern.h"

HINSTANCE hinstance;

b8 tl_platform_initialize(void){
    hinstance = GetModuleHandle(NULL);

    return true;
}

b8 tl_platform_terminate(void){
    return true;
}

#endif // TLPLATFORM_WINDOWS
