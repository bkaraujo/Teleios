#ifndef TELEIOS_PLATFORM_WINDOW
#define TELEIOS_PLATFORM_WINDOW

#include "teleios/types.h"

typedef struct {
    const char* title;
    u32 width;
    u32 height;
} TLCreateWindowInfo;

void tl_platform_window_create(TLCreateWindowInfo* info);
void tl_platform_window_destroy();
void tl_platform_window_show();
void tl_platform_window_hide();
void tl_platform_window_update();

#endif // TELEIOS_PLATFORM_TIME