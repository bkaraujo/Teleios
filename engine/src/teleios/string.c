#include "teleios/teleios.h"
#include <string.h>

u64 tl_string_length(const char* string) {
    return strlen(string);
}

b8 tl_string_equals(const char* str0, const char* str1) {
    return strcmp(str0, str1) == 0;
}

b8 tl_string_begin_with(const char* string, const char* desired) {
    TLDPUSH;
    
    u64 len1 = tl_string_length(string);
    u64 len2 = tl_string_length(desired);
    if (len2 > len1) { return false; }

    for (u32 i = 0 ; i < len2; ++i) {
        if (string[i] != desired[i]) {
            TLDPOP;
            return false;
        }
    }

    TLDPOP;
    return true;
}

b8 tl_string_end_with(const char* string, const char* desired) {
    TLDPUSH;
    
    u64 len1 = tl_string_length(string);
    u64 len2 = tl_string_length(desired);
    if (len2 > len1) { return false; }

    u32 index = len1 - len2;
    for (u32 i = 0 ; i < len2; ++i) {
        if (string[index++] != desired[i]) {
            TLDPOP;
            return false;
        }
    }

    TLDPOP;
    return true;
}

const char* tl_string_join(const char* string, const char* appended) {
    TLDPUSH;

    u64 len1 = tl_string_length(string);
    u64 len2 = tl_string_length(appended);
    char* joined = tl_memory_alloc(TL_MEMORY_STRING, len1+len2);

    strcat(joined, string);
    strcat(joined, appended);

    TLDPOP;
    return joined;
}

void tl_string_free(const char* string) {
    TLDPUSH;
    
    u64 lentgh = tl_string_length(string);
    tl_memory_free(TL_MEMORY_STRING, lentgh, (void*)string);

    TLDPOP;
}

const char* tl_string_clone(const char* string) {
    TLDPUSH;
    
    u64 length = tl_string_length(string);
    void* cloned = tl_memory_alloc(TL_MEMORY_STRING, length);
    tl_memory_copy((void*) string, length, cloned);

    TLDPOP;
    return cloned;
}

i32 tl_string_index_of(const char* str, char c) {
    TLDPUSH;
    if (str == NULL) { TLDPOP; return -1; }

    u64 length = tl_string_length(str);
    if (length == 0) { TLDPOP; return -1; }
    if (str[0] == '\0') { TLDPOP; return -1; }
    
    for (i32 i = 0; i < length; ++i) {
        if (str[i] == c) {
            TLDPOP;
            return i;
        }
    }

    TLDPOP;
    return -1;
}