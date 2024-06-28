#ifndef TELEIOS_PLATFORM_MEMORY
#define TELEIOS_PLATFORM_MEMORY

#include "teleios/types.h"

void* tl_platform_memory_halloc(u64 size);
void tl_platform_memory_hfree(void* block);
void* tl_platform_memory_salloc(u64 size);
void tl_platform_memory_sfree(void* block);
void tl_platform_memory_set(void* block, u64 size,i32 value);
void tl_platform_memory_copy(void* source, void* target, u64 size);

#endif // TELEIOS_PLATFORM_MEMORY