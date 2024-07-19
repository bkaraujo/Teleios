#ifndef TELEIOS_MAIN
#define TELEIOS_MAIN

#include "teleios/engine.h"
#include "teleios/teleios.h"

void tl_application_configure(TLAppSpecification* spec);

int main() {
    if (!tl_engine_pre_initialize()) {
        TLERROR("Engine failed to initialize underlying platform");
        return 9;
    }

    TLTimer* timer = tl_chrono_timer_create();

    if (!tl_engine_initialize()) {
        TLERROR("Engine failed to initialize");
        if (!tl_engine_terminate()) {
            TLERROR("Engine failed to terminate");
            return 99;
        }

        return 90;
    }

    TLAppSpecification specification = { 0 };
    tl_application_configure(&specification);
    if (!tl_engine_configure(&specification)) {
        TLERROR("Engine failed to configure");
        if (!tl_engine_terminate()) {
            TLERROR("Engine failed to terminate");
            return 99;
        }

        return 91;
    }

    tl_chrono_timer_update(timer);
    TLINFO("Engine initialized in %.3f ms", tl_chrono_timer_millis(timer));
    tl_chrono_timer_destroy(timer);
    timer = NULL;
    
    if (!tl_engine_run()) {
        TLERROR("Engine failed to run");
        if (!tl_engine_terminate()) {
            TLERROR("Engine failed to terminate");
            return 99;
        }

        return 92;
    }

    if (!tl_engine_terminate()) {
        TLERROR("Engine failed to terminate");
        return 99;
    }

    return 0;
}

#endif // TELEIOS_MAIN