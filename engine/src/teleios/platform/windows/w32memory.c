#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include "teleios/platform/memory.h"
#include "teleios/platform/windows/w32extern.h"


void* tl_platform_memory_alloc(u64 size) {
    return (void*)HeapAlloc(heap, HEAP_ZERO_MEMORY, size);
}

void tl_platform_memory_free(void* block) {
    HeapFree(heap, HEAP_NO_SERIALIZE, block);
}

void tl_platform_memory_set(void* block, u64 size, i32 value) {
    return memset(block, value, size);
}

void tl_platform_memory_copy(void* source, void* target, u64 size) {
    return memcpy(target, source, size);
}

#endif // TLPLATFORM_WINDOWS
