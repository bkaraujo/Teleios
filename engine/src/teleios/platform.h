#ifndef TELEIOS_PLATFORM
#define TELEIOS_PLATFORM

#include "teleios/types.h"
// #####################################################################################################
//
//                                           M E M O R Y
//
// #####################################################################################################
void* tl_platform_memory_halloc(u64 size);
void tl_platform_memory_hfree(void* block);
void* tl_platform_memory_salloc(u64 size);
void tl_platform_memory_sfree(void* block);
void tl_platform_memory_set(void* block, u64 size, i32 value);
void tl_platform_memory_copy(void* source, void* target, u64 size);
// #####################################################################################################
//
//                                           C O N S O L E
//
// #####################################################################################################
void tl_platform_console(u8 level, const char* message);
// #####################################################################################################
//
//                                           C H R O N O
//
// #####################################################################################################
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
u64 tl_platform_time_epoch(void);

typedef struct {
    u64 start;
    u64 update;
} TLTimer;

void tl_platform_timer_start(TLTimer* timer);
void tl_platform_timer_update(TLTimer* timer);
u64 tl_platform_timer_micros(TLTimer* timer);
f64 tl_platform_timer_millis(TLTimer* timer);
f64 tl_platform_timer_seconds(TLTimer* timer);
// #####################################################################################################
//
//                                           W I N D O W
//
// #####################################################################################################
typedef struct {
    const char* title;
    u32 width;
    u32 height;
} TLCreateWindowInfo;
void* tl_platform_window_handle(void);
void tl_platform_window_create(TLCreateWindowInfo* info);
void tl_platform_window_destroy();
void tl_platform_window_show();
void tl_platform_window_hide();
void tl_platform_window_update();
// #####################################################################################################
//
//                                           L I F E C Y C L E
//
// #####################################################################################################
void* tl_platform_handle(void);
b8 tl_platform_initialize(void);
b8 tl_platform_terminate(void);

#endif // TELEIOS_PLATFORM