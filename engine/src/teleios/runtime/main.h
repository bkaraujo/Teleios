#ifndef TELEIOS_MAIN
#define TELEIOS_MAIN

#include "teleios/runtime/engine.h"
#include "teleios/runtime/teleios.h"

void tl_application_configure(TLAppSpecification* spec);
b8 tl_application_initialize(void);
b8 tl_application_terminate(void);

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

        return 90;
    }

    tl_chrono_timer_update(timer);
    TLINFO("Engine initialized in %.3f ms", tl_chrono_timer_millis(timer));

    tl_chrono_timer_start(timer);
    if (!tl_application_initialize()) {
        TLERROR("Applicaiton failed to initialize");
        if (!tl_engine_terminate()) {
            TLERROR("Engine failed to terminate");
            return 99;
        }

        return 90;
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

        return 91;
    }

    if (!tl_application_terminate()) {
        TLERROR("Applicaiton failed to terminate");
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