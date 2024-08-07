#ifndef TELEIOS_MESSAGING
#define TELEIOS_MESSAGING

#include "teleios/runtime/types.h"

b8 tl_messaging_initialize(void);
b8 tl_messaging_terminate(void);

typedef TLMessageChain(*TLMessageHandler)(const u16 code, const TLMessage* message);

void tl_messaging_post(const u16 code, const TLMessage* message);
void tl_messaging_subscribe(const u16 code, TLMessageHandler handler);

#endif // TELEIOS_MESSAGING
