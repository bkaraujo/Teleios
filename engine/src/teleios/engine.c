#include "teleios/engine.h"
#include "teleios/logger.h"
#include "teleios/platform/lifecycle.h"

TLAPI b8 tl_engine_initialize(void) {
    TLTRACE("tl_engine_initialize(void)");
    if (!tl_platform_initialize()) {
        TLERROR("Platform failed to initialize");
        return false;
    }

    return true;
}

TLAPI b8 tl_engine_run(void) {
    TLTRACE("tl_engine_run(void)");
    return true;
}

TLAPI b8 tl_engine_terminate(void) {
    TLTRACE("tl_engine_terminate(void)");
    if (!tl_platform_terminate()) {
        TLERROR("Platform failed to terminate");
        return false;
    }

    return true;
}