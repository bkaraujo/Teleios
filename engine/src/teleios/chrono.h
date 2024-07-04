#ifndef TELEIOS_CHRONO
#define TELEIOS_CHRONO

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

TLAPI void tl_chrono_time_now(TLTime* time);
TLAPI u64 tl_chrono_time_epoch(void);

typedef struct {
    u64 start;
    u64 update;
} TLTimer;

TLAPI void tl_chrono_timer_start(TLTimer* timer);
TLAPI void tl_chrono_timer_update(TLTimer* timer);
TLAPI u64 tl_chrono_timer_micros(TLTimer* timer);
TLAPI f64 tl_chrono_timer_millis(TLTimer* timer);
TLAPI f64 tl_chrono_timer_seconds(TLTimer* timer);

#endif // TELEIOS_CHRONO