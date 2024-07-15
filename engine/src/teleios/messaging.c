#include "teleios/teleios.h"
#include "teleios/messagingcodes.h"

static TLList* of_codes;

b8 tl_messaging_initialize(void) {
    TLDPUSH;

    of_codes = tl_list_create();
    for (int i = 0; i < TL_MESSAGE_ALL_KNOWN; ++i) {
        TLList* created = tl_list_create();
        tl_list_add(of_codes, created);
    }

    TLDPOP;
    return true;
}

void tl_messaging_subscribe(const u16 code, TLMessageHandler handler) {
    TLDPUSH;

    // =================================================================
    // Ensure the handler function exists
    // Ensure code is within capacity
    // =================================================================
    if (handler == NULL) TLFATAL("Message handler is NULL");
    if (code > TL_MESSAGE_ALL_KNOWN) TLFATAL("Message code %d beyond maximum capacity %llu", code, TL_MESSAGE_ALL_KNOWN);
    // =================================================================
    // Iterate over the of_codes list to get the desired code handling list
    // Insert the PFN into the of_listener list
    // =================================================================
    if (code != TL_MESSAGE_ALL_KNOWN) {
        TLListNode* of_listener = of_codes->head;
        for (int i = 0; i < code; ++i) of_listener = of_listener->next;
        tl_list_add(of_listener->payload, (void*)handler);

        TLDPOP;
        return;
    }
    // =================================================================
    // Register the hander for all possible messages
    // =================================================================
    TLListNode* of_listener = of_codes->head;
    for (int i = 0; i < TL_MESSAGE_ALL_KNOWN; ++i) {
        tl_list_add(of_listener->payload, (void*)handler);
        of_listener = of_listener->next;
    }

    TLDPOP;
}

void tl_messaging_post(const u16 code, const TLMessage* message) {
    TLDPUSH;

    // =================================================================
    // Ensure code is within capacity
    // =================================================================
    if (code >= TL_MESSAGE_ALL_KNOWN) TLFATAL("Message code %d beyond maximum capacity %llu", code, TL_MESSAGE_ALL_KNOWN);
    // =================================================================
    // Iterate over the of_codes list to get the desired code handling list
    // =================================================================
    TLListNode* of_listener = of_codes->head;
    for (int i = 0; i < code; ++i) of_listener = of_listener->next;
    // =================================================================
    // For each entry of of_listener ofer the message for processing
    // Interrupt the processing if a handler respond with TL_MESSAGE_COMSUMED
    // =================================================================
    TLListNode* node = ((TLList*)of_listener->payload)->head;
    while (node != NULL) {
        TLMessageHandler handler = (TLMessageHandler) node->payload;
        if (handler(code, message) == TL_MESSAGE_COMSUMED) break;
        node = node->next;
    }

    TLDPOP;
}

b8 tl_messaging_terminate(void) {
    TLDPUSH;

    if (of_codes != NULL) {
        TLListNode* node = of_codes->head;
        while (node != NULL) {
            tl_list_destroy((TLList*)node->payload, tl_list_purger_noop);
            node = node->next;
        }

        tl_list_destroy(of_codes, tl_list_purger_noop);
    }

    TLDPOP;
    return true;
}