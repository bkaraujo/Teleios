#include "teleios/teleios.h"
#include "teleios/messagingcodes.h"

static TLMap* of_codes;

b8 tl_messaging_initialize(void) {
    TLDPUSH;
    of_codes = tl_map_create();
    TLDRV(true);
}

void tl_messaging_subscribe(const u16 code, TLMessageHandler handler) {
    TLDPUSH;

    if (handler == NULL) TLFATAL("Message handler is NULL");
    if (code > TL_MESSAGE_ALL_KNOWN) TLFATAL("Message code %d beyond maximum capacity %llu", code, TL_MESSAGE_ALL_KNOWN);
    if (code != TL_MESSAGE_ALL_KNOWN) {
        tl_map_put(of_codes, code, (void*) handler);
        TLDRE;
    }
    
    for (u16 i = 0 ; i < TL_MESSAGE_ALL_KNOWN ; ++i) {
        tl_map_put(of_codes, i, (void*)handler);
    }

    TLDRE;
}

void tl_messaging_post(const u16 code, const TLMessage* message) {
    TLDPUSH;

    if (code >= TL_MESSAGE_ALL_KNOWN) TLFATAL("Message code %d beyond maximum capacity %llu", code, TL_MESSAGE_ALL_KNOWN);

    TLList* values = tl_map_values(of_codes, code);
    TLListNode* node = values->head;
    while (node != NULL) {
        TLMessageHandler handler = (TLMessageHandler) node->payload;
        if (handler(code, message) == TL_MESSAGE_COMSUMED) break;
        node = node->next;
    }

    TLDRE;
}

b8 tl_messaging_terminate(void) {
    TLDPUSH;
    
    if (of_codes != NULL)  {
        tl_map_destroy(of_codes, tl_list_purger_noop);
        of_codes = NULL;
    }
    
    TLDRV(true);
}