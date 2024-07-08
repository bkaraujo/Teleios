#ifndef TELEIOS_PLATFORM
#define TELEIOS_PLATFORM

#include "teleios/types.h"
// #####################################################################################################
//
//                                           M E M O R Y
//
// #####################################################################################################
void* tl_platform_memory_halloc(u64 size);
void tl_platform_memory_hfree(void* block);
void* tl_platform_memory_salloc(u64 size);
void tl_platform_memory_sfree(void* block);
void tl_platform_memory_set(void* block, u64 size, i32 value);
void tl_platform_memory_copy(void* source, u64 size, void* target);
// #####################################################################################################
//
//                                           C O N S O L E
//
// #####################################################################################################
void tl_platform_console(u8 level, const char* message);
// #####################################################################################################
//
//                                           W I N D O W
//
// #####################################################################################################
void* tl_platform_window_handle(void);
void tl_platform_window_create(TLWindowCreateInfo* info);
void tl_platform_window_destroy();
void tl_platform_window_show();
void tl_platform_window_hide();
void tl_platform_window_update();
// #####################################################################################################
//
//                                           L I F E C Y C L E
//
// #####################################################################################################
void* tl_platform_handle(void);
b8 tl_platform_initialize(void);
b8 tl_platform_terminate(void);

#endif // TELEIOS_PLATFORM