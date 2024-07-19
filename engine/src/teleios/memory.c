#include "teleios/teleios.h"
#include "teleios/platform.h"

typedef struct {
    u64 allocated;
    u64 oftype[TL_MEMORY_MAXIMUM];
} TLMemoryRegistry;

static TLMemoryRegistry registry;

b8 tl_memory_initialize(void) {
    TLDPUSH;

    tl_platform_memory_set((void*) &registry, sizeof(TLMemoryRegistry), 0);

    TLDPOP;
    return true;
}

void* tl_memory_alloc(TLMemoryType type, u64 size) {
    TLDPUSH;
    
    void* block = tl_platform_memory_halloc(size);
    if (block == NULL) {
        TLFATAL("Failed to allocate %llu bytes", size);
        TLDPOP;
        return NULL;
    }

    registry.allocated += size;
    registry.oftype[type] += size;

    TLDPOP;
    return block;
}

void tl_memory_free(TLMemoryType type, u64 size, void* pointer) {
    TLDPUSH;

    if (pointer == NULL) { 
        TLFATAL("pointer is null");
        TLDPOP;
        return; 
    }

    tl_platform_memory_hfree(pointer);

    registry.allocated -= size;
    registry.oftype[type] -= size;

    TLDPOP;
}

void tl_memory_zero(void* pointer, u64 size) {
    TLDPUSH;

    if (pointer == NULL) {
        TLFATAL("pointer is null");
        TLDPOP;
        return;
    }

    tl_platform_memory_set(pointer, size, 0);

    TLDPOP;
}

void tl_memory_copy(void* source, u64 size, void* target) {
    TLDPUSH;

    if (size == 0) {
        TLFATAL("size is zero");
        TLDPOP;
        return;
    }

    if (source == NULL) {
        TLFATAL("source is null");
        TLDPOP;
        return;
    }

    if (target == NULL) {
        TLFATAL("target is null");
        TLDPOP;
        return;
    }

    tl_platform_memory_copy(source, size, target);

    TLDPOP;
}

void tl_memory_set(void* pointer, u64 size, i32 value) {
    TLDPUSH;

    if (pointer == NULL) {
        TLFATAL("pointer is null");
        TLDPOP;
        return;
    }

    tl_platform_memory_set(pointer, size, value);

    TLDPOP;
}

static const char* tl_memory_label(TLMemoryType type) {
    switch (type) {
        case TL_MEMORY_ULID: return "TL_MEMORY_ULID";
        case TL_MEMORY_TIMER: return "TL_MEMORY_TIMER";
        case TL_MEMORY_AUDIO: return "TL_MEMORY_AUDIO";
        case TL_MEMORY_STRING: return "TL_MEMORY_STRING";
        case TL_MEMORY_GRAPHICS: return "TL_MEMORY_GRAPHICS";
        case TL_MEMORY_RESOURCE: return "TL_MEMORY_RESOURCE";
        case TL_MEMORY_FILESYSTEM: return "TL_MEMORY_FILESYSTEM";
        case TL_MEMORY_ECS_ENTITY: return "TL_MEMORY_ECS_ENTITY";
        case TL_MEMORY_ECS_COMPONENT: return "TL_MEMORY_ECS_COMPONENT";
        case TL_MEMORY_CONTAINER_MAP: return "TL_MEMORY_CONTAINER_MAP";
        case TL_MEMORY_CONTAINER_LIST: return "TL_MEMORY_CONTAINER_LIST";
        case TL_MEMORY_CONTAINER_NODE: return "TL_MEMORY_CONTAINER_NODE";
        
        default: return "????";
    }
}

b8 tl_memory_terminate(void) {
    TLDPUSH;

    if (registry.allocated != 0) {
        TLERROR("Memory leaked");
        for (int i = 0; i < TL_MEMORY_MAXIMUM; ++i) {
            if (registry.oftype[i] == 0) continue;

            TLERROR("%s: %llu", tl_memory_label(i), registry.oftype[i]);
        }
    }

    TLDPOP;
    return true;
}