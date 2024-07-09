#include "teleios/teleios.h"

typedef struct {
    u64 allocated;
    u64 oftype[TL_MEMORY_MAXIMUM];
} TLMemoryRegistry;

static TLMemoryRegistry registry;

b8 tl_memory_initialize(void) {
    TLDIAGNOSTICS_PUSH;

    tl_platform_memory_set((void*) &registry, sizeof(TLMemoryRegistry), 0);

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

    registry.allocated += size;
    registry.oftype[type] += size;

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

    registry.allocated -= size;
    registry.oftype[type] -= size;

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

void tl_memory_copy(void* source, u64 size, void* target) {
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

    tl_platform_memory_copy(source, size, target);

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

static const char* tl_memory_label(TLMemoryType type) {
    switch (type) {
        case TL_MEMORY_CONTAINER: return "TL_MEMORY_CONTAINER";
        case TL_MEMORY_CONTAINER_NODE: return "TL_MEMORY_CONTAINER_NODE";
        case TL_MEMORY_GRAPHICS: return "TL_MEMORY_GRAPHICS";
        case TL_MEMORY_FILESYSTEM: return "TL_MEMORY_FILESYSTEM";
        case TL_MEMORY_RESOURCE: return "TL_MEMORY_RESOURCE";
        default: return "????";
    }
}

b8 tl_memory_terminate(void) {
    TLDIAGNOSTICS_PUSH;

    if (registry.allocated != 0) {
        TLERROR("Memory leaked");
        for (int i = 0; i < TL_MEMORY_MAXIMUM; ++i) {
            if (registry.oftype[i] == 0) continue;

            TLERROR("%s: %llu", tl_memory_label(i), registry.oftype[i]);
        }
    }

    TLDIAGNOSTICS_POP;
    return true;
}