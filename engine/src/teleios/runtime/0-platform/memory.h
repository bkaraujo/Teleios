#ifndef TELEIOS_RUNTIME_PLATFORM_MEMORY
#define TELEIOS_RUNTIME_PLATFORM_MEMORY

#include "teleios/types.h"

void* tl_platform_memory_halloc(TLMemoryType type, u64 size);
void tl_platform_memory_hfree(void* block);
void* tl_platform_memory_salloc(TLMemoryType type, u64 size);
void tl_platform_memory_sfree(void* block);

void tl_platform_memory_set(void* block, u64 size, i32 value);
void tl_platform_memory_copy(void* source, u64 size, void* target);

#endif // TELEIOS_RUNTIME_PLATFORM_MEMORY