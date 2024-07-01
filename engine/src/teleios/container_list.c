#include "teleios/container.h"
#include "teleios/memory/allocator.h"
#include "teleios/logger.h"
#include "teleios/diagnostic/stacktrace.h"

TLList* tl_list_create(void) {
    TLDIAGNOSTICS_PUSH;
    TLList* list = tl_memory_alloc(TL_MEMORY_CONTAINER, sizeof(TLList));
    if (list == NULL) TLFATAL("Failed to allocate TLList*");

    list->length = 0;

    TLDIAGNOSTICS_POP;
    return list;
}

void  tl_list_add(TLList* list, void* payload) {
    TLDIAGNOSTICS_PUSH;
    if (list == NULL) { TLDIAGNOSTICS_POP; return; }

    TLListNode* node = tl_memory_alloc(TL_MEMORY_CONTAINER_NODE, sizeof(TLListNode));
    node->payload = payload;
    node->next = NULL;
    node->previous = NULL;

    if (list->length == 0) {
        list->head = node;
        list->tail = node;
        list->length++;
    
        TLDIAGNOSTICS_POP;
        return;
    }

    node->previous = list->tail;
    list->tail->next = node;
    list->tail = node;
    list->length++;

    TLDIAGNOSTICS_POP;
}

void tl_list_rem(TLList* list, void* payload) {
    TLDIAGNOSTICS_PUSH;
    if (list == NULL) { TLDIAGNOSTICS_POP; return; }
 
    TLListNode* node = list->head;
    while (node != NULL) {
        if (node->payload == payload) {
            if (node->next != NULL) node->next->previous = node->previous;
            if (node->previous != NULL) node->previous->next = node->next;

            list->length--;
            TLDIAGNOSTICS_POP;
            return;
        }

        node = node->next;
    }

    TLDIAGNOSTICS_POP;
}

void  tl_list_destroy(TLList* list, b8(*purger)(void*)) {
    TLDIAGNOSTICS_PUSH;
    if (list == NULL) { TLDIAGNOSTICS_POP; return; }

    if (list->length > 0) {
        if (purger == NULL) { TLFATAL("Impossible to destroy a populated list without a purger function"); }
        
        TLListNode* node = list->head;
        while (node != NULL) {
            TLListNode* current = node;
            node = node->next;

            tl_memory_free(TL_MEMORY_CONTAINER_NODE, sizeof(TLListNode), current);
        }
    }
 
    tl_memory_free(TL_MEMORY_CONTAINER, sizeof(TLList), list);
    TLDIAGNOSTICS_POP;
}


b8 tl_list_purger_noop(void* pointer) {
    return true;
}