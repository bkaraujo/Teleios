#ifndef TELEIOS_PLATFORM
#define TELEIOS_PLATFORM

#include "teleios/runtime/types.h"

void tl_platform_console(u8 level, const char* message);

void* tl_platform_memory_halloc(TLMemoryType type, u64 size);
void tl_platform_memory_hfree(void* block);
void* tl_platform_memory_salloc(TLMemoryType type, u64 size);
void tl_platform_memory_sfree(void* block);
void tl_platform_memory_set(void* block, u64 size, i32 value);
void tl_platform_memory_copy(void* source, u64 size, void* target);

void* tl_platform_window_handle(void);
void tl_platform_window_create(TLWindowCreateInfo* info);
void tl_platform_window_destroy();
void tl_platform_window_show();
void tl_platform_window_hide();
void tl_platform_window_update();

i32 tl_platform_entropy(void* buffer, i32 length);

void* tl_platform_handle(void);
b8 tl_platform_initialize(void);
b8 tl_platform_terminate(void);
#endif // TELEIOS_PLATFORM