#ifndef TELEIOS_STRING
#define TELEIOS_STRING

#include "teleios/runtime/types.h"

u64 tl_string_length(const char* string);
b8 tl_string_equals(const char* str0, const char* str1);
b8 tl_string_begin_with(const char* string, const char* desired);
b8 tl_string_end_with(const char* string, const char* desired);
const char* tl_string_join(const char* string, const char* appended);
void tl_string_free(const char* string);
const char* tl_string_clone(const char* string);
i32 tl_string_index_of(const char* str, char c);

#endif // TELEIOS_STRING