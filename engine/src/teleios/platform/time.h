#ifndef TELEIOS_PLATFORM_TIME
#define TELEIOS_PLATFORM_TIME

#include "teleios/types.h"

typedef struct {
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 seconds;
    u16 millis;
} TLTime;

void tl_platform_time_now(TLTime* time);

typedef struct {
    u64 start;
    u64 update;
} TLTimer;

void tl_platform_timer_start(TLTimer* timer);
void tl_platform_timer_update(TLTimer* timer);
u64 tl_platform_timer_micros(TLTimer* timer);
f64 tl_platform_timer_millis(TLTimer* timer);
f64 tl_platform_timer_seconds(TLTimer* timer);

#endif // TELEIOS_PLATFORM_TIME