#ifndef TELEIOS_RUNTIME_PLATFORM
#define TELEIOS_RUNTIME_PLATFORM

#include "teleios/types.h"

#include "teleios/runtime/0-platform/memory.h"
#include "teleios/runtime/0-platform/crypto.h"
#include "teleios/runtime/0-platform/window.h"
#include "teleios/runtime/0-platform/console.h"
#include "teleios/runtime/0-platform/filesystem.h"

void* tl_platform_handle(void);
b8 tl_platform_initialize(void);
b8 tl_platform_terminate(void);

#endif // TELEIOS_RUNTIME_PLATFORM