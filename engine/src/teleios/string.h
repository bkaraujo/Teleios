#ifndef TELEIOS_STRING
#define TELEIOS_STRING

#include "teleios/types.h"

TLAPI u64 tl_string_size(const char* string);

TLAPI b8 tl_string_equal(const char* str0, const char* str1);
TLAPI b8 tl_string_begin_with(const char* string, const char* desired);
TLAPI b8 tl_string_end_with(const char* string, const char* desired);
TLAPI const char* tl_string_join(const char* string, const char* appended);
TLAPI void tl_string_free(const char* string);
TLAPI const char* tl_string_clone(const char* string);

#endif // TELEIOS_STRING