#include "teleios/runtime/teleios.h"
#include "teleios/runtime/platform.h"

b8 tl_memory_initialize(void) {
    return true;
}

void* tl_memory_alloc(TLMemoryType type, u64 size) {
    TLDPUSH;
    
    void* block = tl_platform_memory_halloc(type, size);
    if (block == NULL) TLFATAL("Failed to allocate %llu bytes", size);

    TLDRV(block);
}

void tl_memory_free(void* pointer) {
    TLDPUSH;

    if (pointer == NULL) TLFATAL("pointer is null");
    tl_platform_memory_hfree(pointer);

    TLDRE;    
}

void tl_memory_zero(void* pointer, u64 size) {
    TLDPUSH;

    if (pointer == NULL) TLFATAL("pointer is null");
    tl_platform_memory_set(pointer, size, 0);

    TLDRE;
}

void tl_memory_copy(void* source, u64 size, void* target) {
    TLDPUSH;

    if (size == 0) TLFATAL("size is zero");
    if (source == NULL) TLFATAL("source is null");
    if (target == NULL) TLFATAL("target is null");
    tl_platform_memory_copy(source, size, target);

    TLDRE;
}

void tl_memory_set(void* pointer, u64 size, i32 value) {
    TLDPUSH;

    if (pointer == NULL) TLFATAL("pointer is null");
    tl_platform_memory_set(pointer, size, value);

    TLDRE;
}

b8 tl_memory_terminate(void) {
    return true;
}