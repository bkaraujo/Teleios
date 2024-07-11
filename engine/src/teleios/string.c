#include "teleios/teleios.h"
#include <string.h>

u64 tl_string_length(const char* string) {
    return strlen(string);
}

b8 tl_string_equal(const char* str0, const char* str1) {
    return strcmp(str0, str1) == 0;
}

b8 tl_string_begin_with(const char* string, const char* desired) {
    TLDIAGNOSTICS_PUSH;
    
    u64 len1 = tl_string_length(string);
    u64 len2 = tl_string_length(desired);
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

b8 tl_string_end_with(const char* string, const char* desired) {
    TLDIAGNOSTICS_PUSH;
    
    u64 len1 = tl_string_length(string);
    u64 len2 = tl_string_length(desired);
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

const char* tl_string_join(const char* string, const char* appended) {
    TLDIAGNOSTICS_PUSH;

    u64 len1 = tl_string_length(string);
    u64 len2 = tl_string_length(appended);
    char* joined = tl_memory_alloc(TL_MEMORY_STRING, len1+len2);

    strcat(joined, string);
    strcat(joined, appended);

    TLDIAGNOSTICS_POP;
    return joined;
}

void tl_string_free(const char* string) {
    TLDIAGNOSTICS_PUSH;
    
    u64 lentgh = tl_string_length(string);
    tl_memory_free(TL_MEMORY_STRING, lentgh, (void*)string);

    TLDIAGNOSTICS_POP;
}

const char* tl_string_clone(const char* string) {
    TLDIAGNOSTICS_PUSH;
    
    u64 length = tl_string_length(string);
    void* cloned = tl_memory_alloc(TL_MEMORY_STRING, length);
    tl_memory_copy((void*) string, length, cloned);

    TLDIAGNOSTICS_POP;
    return cloned;
}

i32 tl_string_index_of(const char* str, char c) {
    TLDIAGNOSTICS_PUSH;
    if (str == NULL) { TLDIAGNOSTICS_POP; return -1; }

    u64 length = tl_string_length(str);
    if (length == 0) { TLDIAGNOSTICS_POP; return -1; }
    if (str[0] == '\0') { TLDIAGNOSTICS_POP; return -1; }
    
    for (i32 i = 0; i < length; ++i) {
        if (str[i] == c) {
            TLDIAGNOSTICS_POP;
            return i;
        }
    }

    TLDIAGNOSTICS_POP;
    return -1;
}