#ifndef TELEIOS_MEMORY
#define TELEIOS_MEMORY

#include "teleios/types.h"

b8 tl_memory_initialize(void);
b8 tl_memory_terminate(void);

void* tl_memory_alloc(TLMemoryType type, u64 size);
void tl_memory_free(TLMemoryType type, u64 size, void* pointer);
void tl_memory_zero(void* pointer, u64 size);
void tl_memory_copy(void* source, void* target, u64 size);
void tl_memory_set(void* pointer, u64 size, i32 value);

#endif // TELEIOS_MEMORY