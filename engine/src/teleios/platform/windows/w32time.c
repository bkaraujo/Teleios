#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include "teleios/platform/time.h"
#include "teleios/platform/windows/w32extern.h"
#include "teleios/diagnostic/stacktrace.h"

void tl_platform_time_now(TLTime* time) {
    TLDIAGNOSTICS_PUSH;
    if (time == NULL) { TLDIAGNOSTICS_POP; return; }

    SYSTEMTIME st; GetLocalTime(&st);

    time->year      = st.wYear;
    time->month     = (u8)st.wMonth;
    time->day       = (u8)st.wDay;
    time->hour      = (u8)st.wHour;
    time->minute    = (u8)st.wMinute;
    time->seconds   = (u8)st.wSecond;
    time->millis    = st.wMilliseconds;
    TLDIAGNOSTICS_POP;
}

u64 tl_platform_time_epoch(void) {
    TLDIAGNOSTICS_PUSH;
    // #################################################################
    // Obtain the system 64bit time
    // #################################################################
    FILETIME stime; GetSystemTimeAsFileTime(&stime);
    ULARGE_INTEGER curr_time_as_uint64;
    curr_time_as_uint64.HighPart = stime.dwHighDateTime;
    curr_time_as_uint64.LowPart = stime.dwLowDateTime;
    // #################################################################
    // Translate the extern unix_epoch to 64bit filetime
    // #################################################################
    FILETIME utime; SystemTimeToFileTime(&unix_epoch, &utime);
    ULARGE_INTEGER unix_epoch_as_uint64;
    unix_epoch_as_uint64.HighPart = utime.dwHighDateTime;
    unix_epoch_as_uint64.LowPart = utime.dwLowDateTime;
    // #################################################################
    // Compute the milliseconds since unix epoch
    // #################################################################
    TLDIAGNOSTICS_POP;
    return (curr_time_as_uint64.QuadPart - unix_epoch_as_uint64.QuadPart) / 10000;
}

void tl_platform_timer_start(TLTimer* timer) {
    TLDIAGNOSTICS_PUSH;
    if (timer == NULL) { TLDIAGNOSTICS_POP; return; }
    LARGE_INTEGER now; QueryPerformanceCounter(&now);
    timer->start = now.QuadPart;
    timer->update = 0;
    TLDIAGNOSTICS_POP;
}

void tl_platform_timer_update(TLTimer* timer){
    TLDIAGNOSTICS_PUSH;
    if (timer == NULL) { TLDIAGNOSTICS_POP; return; }
    LARGE_INTEGER now; QueryPerformanceCounter(&now);
    timer->update = now.QuadPart;
    TLDIAGNOSTICS_POP;
}

u64 tl_platform_timer_micros(TLTimer* timer) {
    TLDIAGNOSTICS_PUSH;
    if (timer == NULL) { TLDIAGNOSTICS_POP; return 0; }
    u64 elapsed = timer->update - timer->start;
    u64 elapsedMicros = (elapsed * 1000000) / frequency.QuadPart; // 100-nanos to micros
    TLDIAGNOSTICS_POP;
    return elapsedMicros;
}

f64 tl_platform_timer_millis(TLTimer* timer) {
    TLDIAGNOSTICS_PUSH;
    if (timer == NULL) { TLDIAGNOSTICS_POP; return 0.0f; }
    f64 elaspsedMillis = tl_platform_timer_micros(timer) / 1000.0f;
    TLDIAGNOSTICS_POP;
    return elaspsedMillis;
}

f64 tl_platform_timer_seconds(TLTimer* timer) {
    TLDIAGNOSTICS_PUSH;
    if (timer == NULL) { TLDIAGNOSTICS_POP; return 0.0f; }
    f64 elaspsedSeconds = tl_platform_timer_millis(timer) / 1000.0f;
    TLDIAGNOSTICS_POP;
    return elaspsedSeconds;
}

#endif // TLPLATFORM_WINDOWS
