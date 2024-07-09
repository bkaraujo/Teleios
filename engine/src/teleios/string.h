#ifndef TELEIOS_STRING
#define TELEIOS_STRING

#include "teleios/types.h"

TLAPI u64 tl_string_size(const char* string);

TLAPI b8 tl_string_equal(const char* str0, const char* str1);
TLAPI b8 tl_string_start_with(const char* string, const char* desired);
TLAPI b8 tl_string_ends_with(const char* string, const char* desired);

#endif // TELEIOS_STRING