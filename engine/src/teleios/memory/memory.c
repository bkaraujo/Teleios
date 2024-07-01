#include "teleios/memory/lifecycle.h"
#include "teleios/memory/allocator.h"
#include "teleios/platform/memory.h"
#include "teleios/logger.h"
#include "teleios/diagnostic/stacktrace.h"

typedef struct {
    u64 allocated;
    u64 oftype[TL_MEMORY_MAXIMUM];
} TLMemoryRegistry;

static TLMemoryRegistry registry = { 0 };

b8 tl_memory_initialize(void) {
    TLDIAGNOSTICS_PUSH;



    TLDIAGNOSTICS_POP;
    return true;
}

void* tl_memory_alloc(TLMemoryType type, u64 size) {
    TLDIAGNOSTICS_PUSH;
    
    void* block = tl_platform_memory_halloc(size);
    if (block == NULL) {
        TLFATAL("Failed to allocate %llu bytes", size);
        TLDIAGNOSTICS_POP;
        return NULL;
    }

    TLDIAGNOSTICS_POP;
    return block;
}

void tl_memory_free(TLMemoryType type, u64 size, void* pointer) {
    TLDIAGNOSTICS_PUSH;

    if (pointer == NULL) { 
        TLFATAL("pointer is null");
        TLDIAGNOSTICS_POP;
        return; 
    }

    tl_platform_memory_hfree(pointer);

    TLDIAGNOSTICS_POP;
}

void tl_memory_zero(void* pointer, u64 size) {
    TLDIAGNOSTICS_PUSH;

    if (pointer == NULL) {
        TLFATAL("pointer is null");
        TLDIAGNOSTICS_POP;
        return;
    }

    tl_platform_memory_set(pointer, size, 0);

    TLDIAGNOSTICS_POP;
}

void tl_memory_copy(void* source, void* target, u64 size) {
    TLDIAGNOSTICS_PUSH;

    if (size == 0) {
        TLFATAL("size is zero");
        TLDIAGNOSTICS_POP;
        return;
    }

    if (source == NULL) {
        TLFATAL("source is null");
        TLDIAGNOSTICS_POP;
        return;
    }

    if (target == NULL) {
        TLFATAL("target is null");
        TLDIAGNOSTICS_POP;
        return;
    }

    tl_platform_memory_copy(source, target, size);

    TLDIAGNOSTICS_POP;
}

void tl_memory_set(void* pointer, u64 size, i32 value) {
    TLDIAGNOSTICS_PUSH;

    if (pointer == NULL) {
        TLFATAL("pointer is null");
        TLDIAGNOSTICS_POP;
        return;
    }

    tl_platform_memory_set(pointer, size, value);

    TLDIAGNOSTICS_POP;
}

b8 tl_memory_terminate(void) {
    TLDIAGNOSTICS_PUSH;



    TLDIAGNOSTICS_POP;
    return true;
}