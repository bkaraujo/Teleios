#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include "malloc.h"
#include "teleios/platform/memory.h"
#include "teleios/platform/windows/w32extern.h"

void* tl_platform_memory_halloc(u64 size) {
    return (void*)HeapAlloc(heap, HEAP_ZERO_MEMORY, size);
}

void tl_platform_memory_hfree(void* block) {
    HeapFree(heap, HEAP_NO_SERIALIZE, block);
}

void* tl_platform_memory_salloc(u64 size) {
    return (void*)_malloca(size);
}

void tl_platform_memory_sfree(void* block) {
    _freea(block);
}

void tl_platform_memory_set(void* block, u64 size, i32 value) {
    memset(block, value, size);
}

void tl_platform_memory_copy(void* source, void* target, u64 size) {
    memcpy(target, source, size);
}

#endif // TLPLATFORM_WINDOWS
