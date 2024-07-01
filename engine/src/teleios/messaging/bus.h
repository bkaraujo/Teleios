#ifndef TELEIOS_MESSAGING_BUS
#define TELEIOS_MESSAGING_BUS

#include "teleios/types.h"

typedef enum {
    TL_MESSAGE_AVALIABLE,
    TL_MESSAGE_COMSUMED,
    TL_MESSAGE_MAXIMUM
} TLMessageChain;

typedef union {
    i64 i64[2];
    u64 u64[2];

    f64 f64[2];

    i32 i32[4];
    u32 u32[4];
    f32 f32[4];

    i16 i16[8];

    u16 u16[8];

    i8 i8[16];
    u8 u8[16];

    char c[16];
} TLMessage;

typedef TLMessageChain(*TLMessageHandler)(const u16 code, const TLMessage* message);

void tl_messaging_post(const u16 code, const TLMessage* message);
void tl_messaging_subscribe(const u16 code, TLMessageHandler handler);

#endif // TELEIOS_MESSAGING_BUS
