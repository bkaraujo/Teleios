#ifndef TELEIOS_PLATFORM_MEMORY
#define TELEIOS_PLATFORM_MEMORY

#include "teleios/types.h"

void* tl_platform_memory_alloc(u64 bytes);
void tl_platform_memory_free(void* block);
void tl_platform_memory_set(void* block, u64 size,i32 value);
void tl_platform_memory_copy(void* source, void* target, u64 size);

#endif // TELEIOS_PLATFORM_MEMORY