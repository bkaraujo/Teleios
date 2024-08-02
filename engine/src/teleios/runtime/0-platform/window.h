#ifndef TELEIOS_RUNTIME_PLATFORM_WINDOW
#define TELEIOS_RUNTIME_PLATFORM_WINDOW

#include "teleios/types.h"

void* tl_platform_window_handle(void);
void tl_platform_window_create(TLWindowCreateInfo* info);
void tl_platform_window_destroy();
void tl_platform_window_show();
void tl_platform_window_hide();
void tl_platform_window_update();

#endif // TELEIOS_RUNTIME_PLATFORM_WINDOW