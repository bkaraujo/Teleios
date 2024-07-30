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
    if (code >= TL_MESSAGE_MAXIMUM) TLFATAL("Invalid message code %d.", code);
    
    tl_map_put(of_codes, code, (void*) handler);

    TLDRE;
}

void tl_messaging_post(const u16 code, const TLMessage* message) {
    TLDPUSH;

    if (code >= TL_MESSAGE_MAXIMUM) TLFATAL("Invalid message code %d.", code);

    TLList* values = tl_map_values(of_codes, code);
    if (values == NULL) TLDRE;
    
    TLListNode* node = values->head;
    while (node != NULL) {
        TLMessageHandler handler = (TLMessageHandler) node->payload;
        if (handler(code, message) == TL_MESSAGE_CHAIN_COMSUMED) break;
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