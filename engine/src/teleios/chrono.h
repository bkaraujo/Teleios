#ifndef TELEIOS_CHRONO
#define TELEIOS_CHRONO

#include "teleios/types.h"

TLAPI void tl_chrono_time_now(TLTime* time);
TLAPI u64 tl_chrono_time_epoch_micros(void);
TLAPI u64 tl_chrono_time_epoch_millis(void);

TLAPI void tl_chrono_timer_start(TLTimer* timer);
TLAPI void tl_chrono_timer_update(TLTimer* timer);
TLAPI u64 tl_chrono_timer_micros(TLTimer* timer);
TLAPI f64 tl_chrono_timer_millis(TLTimer* timer);
TLAPI f64 tl_chrono_timer_seconds(TLTimer* timer);

#endif // TELEIOS_CHRONO