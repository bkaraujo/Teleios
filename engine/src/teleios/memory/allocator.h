#ifndef TELEIOS_MEMORY_ALLOCATOR
#define TELEIOS_MEMORY_ALLOCATOR

#include "teleios/types.h"

typedef enum {
    TL_MEMORY_CONTAINER,
    TL_MEMORY_CONTAINER_NODE,
    TL_MEMORY_MAXIMUM
} TLMemoryType;

void* tl_memory_alloc(TLMemoryType type, u64 size);
void tl_memory_free(TLMemoryType type, u64 size, void* pointer);
void tl_memory_zero(void* pointer, u64 size);
void tl_memory_copy(void* source, void* target, u64 size);
void tl_memory_set(void* pointer, u64 size, i32 value);

#endif // TELEIOS_MEMORY_ALLOCATOR