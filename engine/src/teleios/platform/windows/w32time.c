#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include "teleios/platform/time.h"
#include "teleios/platform/windows/w32extern.h"

void tl_platform_time_now(TLTime* time) {
    if (time == NULL) return;
    SYSTEMTIME st; GetLocalTime(&st);

    time->year      = st.wYear;
    time->month     = (u8)st.wMonth;
    time->day       = (u8)st.wDay;
    time->hour      = (u8)st.wHour;
    time->minute    = (u8)st.wMinute;
    time->seconds   = (u8)st.wSecond;
    time->millis    = st.wMilliseconds;
}

u64 tl_platform_time_epoch(void) {
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
    return (curr_time_as_uint64.QuadPart - unix_epoch_as_uint64.QuadPart) / 10000;
}

void tl_platform_timer_start(TLTimer* timer) {
    if (timer == NULL) return;

    LARGE_INTEGER now; QueryPerformanceCounter(&now);
    timer->start = now.QuadPart;
    timer->update = 0;
}

void tl_platform_timer_update(TLTimer* timer){
    if (timer == NULL) return;

    LARGE_INTEGER now; QueryPerformanceCounter(&now);
    timer->update = now.QuadPart;
}

u64 tl_platform_timer_micros(TLTimer* timer) {
    if (timer == NULL) return 0;
    u64 elapsed = timer->update - timer->start;
    u64 elapsedMicros = elapsed * 1000000; // 100-nanos to micros
    return elapsedMicros / frequency.QuadPart;
}

f64 tl_platform_timer_millis(TLTimer* timer) {
    if (timer == NULL) return 0;
    return tl_platform_timer_micros(timer) / 1000.0f;
}

f64 tl_platform_timer_seconds(TLTimer* timer) {
    if (timer == NULL) return 0;
    return tl_platform_timer_millis(timer) / 1000.0f;
}

#endif // TLPLATFORM_WINDOWS
