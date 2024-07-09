#include "teleios/teleios.h"
#include <string.h>

TLAPI u64 tl_string_size(const char* string) {
    return strlen(string);
}

TLAPI b8 tl_string_equal(const char* str0, const char* str1) {
    return strcmp(str0, str1) == 0;
}

TLAPI b8 tl_string_begin_with(const char* string, const char* desired) {
    TLDIAGNOSTICS_PUSH;
    
    u64 len1 = tl_string_size(string);
    u64 len2 = tl_string_size(desired);
    if (len2 > len1) { return false; }

    for (u32 i = 0 ; i < len2; ++i) {
        if (string[i] != desired[i]) {
            TLDIAGNOSTICS_POP;
            return false;
        }
    }

    TLDIAGNOSTICS_POP;
    return true;
}

TLAPI b8 tl_string_end_with(const char* string, const char* desired) {
    TLDIAGNOSTICS_PUSH;
    
    u64 len1 = tl_string_size(string);
    u64 len2 = tl_string_size(desired);
    if (len2 > len1) { return false; }

    u32 index = len1 - len2;
    for (u32 i = 0 ; i < len2; ++i) {
        if (string[index++] != desired[i]) {
            TLDIAGNOSTICS_POP;
            return false;
        }
    }

    TLDIAGNOSTICS_POP;
    return true;
}

TLAPI const char* tl_string_join(const char* string, const char* appended) {
    TLDIAGNOSTICS_PUSH;

    u64 len1 = tl_string_size(string);
    u64 len2 = tl_string_size(appended);
    char* joined = tl_memory_alloc(TL_MEMORY_STRING, len1+len2);

    strcat(joined, string);
    strcat(joined, appended);

    TLDIAGNOSTICS_POP;
    return joined;
}

TLAPI void tl_string_free(const char* string) {
    TLDIAGNOSTICS_PUSH;
    
    u64 lentgh = tl_string_size(string);
    tl_memory_free(TL_MEMORY_STRING, lentgh, (void*)string);

    TLDIAGNOSTICS_POP;
}

TLAPI const char* tl_string_clone(const char* string) {
    TLDIAGNOSTICS_PUSH;
    
    u64 length = tl_string_size(string);
    void* cloned = tl_memory_alloc(TL_MEMORY_STRING, length);
    tl_memory_copy((void*) string, length, cloned);

    TLDIAGNOSTICS_POP;
    return cloned;
}