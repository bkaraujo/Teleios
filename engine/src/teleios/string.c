#include "teleios/teleios.h"
#include <string.h>

TLAPI u64 tl_string_size(const char* string) {
    return strlen(string);
}

TLAPI b8 tl_string_equal(const char* str0, const char* str1) {
    return strcmp(str0, str1) == 0;
}

TLAPI b8 tl_string_start_with(const char* string, const char* desired) {
    return false;
}

TLAPI b8 tl_string_ends_with(const char* string, const char* desired) {
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