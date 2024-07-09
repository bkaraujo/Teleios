#ifndef TELEIOS_STRING
#define TELEIOS_STRING

#include "teleios/types.h"

u64 tl_string_size(const char* string);
b8 tl_string_equal(const char* str0, const char* str1);
b8 tl_string_begin_with(const char* string, const char* desired);
b8 tl_string_end_with(const char* string, const char* desired);
const char* tl_string_join(const char* string, const char* appended);
void tl_string_free(const char* string);
const char* tl_string_clone(const char* string);

#endif // TELEIOS_STRING