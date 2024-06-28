#ifndef TELEIOS_MAIN
#define TELEIOS_MAIN

#include "teleios/logger.h"
#include "teleios/engine.h"

int main() {
    if (!tl_engine_initialize()) {
        TLERROR("Engine failed to initialize");
        if (!tl_engine_terminate()) {
            TLERROR("Engine failed to terminate");
            return 99;
        }

        return 90;
    }


    if (!tl_engine_run()) {
        TLERROR("Engine failed to run");
        if (!tl_engine_terminate()) {
            TLERROR("Engine failed to terminate");
            return 99;
        }

        return 91;
    }

    if (!tl_engine_terminate()) {
        TLERROR("Engine failed to terminate");
        return 99;
    }

    return 0;
}

#endif // TELEIOS_MAIN