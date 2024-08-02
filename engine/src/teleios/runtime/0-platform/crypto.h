#ifndef TELEIOS_RUNTIME_PLATFORM_CRYPTO
#define TELEIOS_RUNTIME_PLATFORM_CRYPTO

#include "teleios/types.h"

i32 tl_platform_entropy(void* buffer, i32 length);

#endif // TELEIOS_RUNTIME_PLATFORM_CRYPTO