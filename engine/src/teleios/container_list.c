#include "teleios/teleios.h"

TLList* tl_list_create(void) {
    TLDPUSH;

    TLList* list = tl_memory_alloc(TL_MEMORY_CONTAINER_LIST, sizeof(TLList));

    TLDPOP;
    return list;
}

void  tl_list_add(TLList* list, void* payload) {
    TLDPUSH;
    if (list == NULL) { TLWARN("List is NULl"); TLDPOP; return; }

    TLListNode* node = tl_memory_alloc(TL_MEMORY_CONTAINER_LIST_ENTRY, sizeof(TLListNode));
    node->payload = payload;
    node->next = NULL;
    node->previous = NULL;

    if (list->length == 0) {
        list->head = node;
        list->tail = node;
        list->length++;
    
        TLDPOP;
        return;
    }

    node->previous = list->tail;
    list->tail->next = node;
    list->tail = node;
    list->length++;

    TLDPOP;
}

void tl_list_rem(TLList* list, void* payload) {
    TLDPUSH;
    if (list == NULL) { TLWARN("List is NULl"); TLDPOP; return; }
 
    TLListNode* node = list->head;
    while (node != NULL) {
        if (node->payload == payload) {
            if (node->next != NULL) node->next->previous = node->previous;
            if (node->previous != NULL) node->previous->next = node->next;

            list->length--;
            
            if (list->length == 0) {
                list->head = NULL;
                list->tail = NULL;
            }

            TLDPOP;
            return;
        }

        node = node->next;
    }

    TLWARN("Payload %p not found", payload);
    TLDPOP;
}

void  tl_list_destroy(TLList* list, b8(*purger)(void*)) {
    TLDPUSH;
    if (list == NULL) { TLWARN("List is NULl"); TLDPOP; return; }

    if (list->length > 0) {
        if (purger == NULL) { TLFATAL("Impossible to destroy a populated list without a purger function"); }
        
        TLListNode* node = list->head;
        while (node != NULL) {
            TLListNode* next = node->next;
            tl_memory_free(TL_MEMORY_CONTAINER_LIST_ENTRY, sizeof(TLListNode), node);
            node = next;
        }
    }
 
    tl_memory_free(TL_MEMORY_CONTAINER_LIST, sizeof(TLList), list);
    TLDPOP;
}

b8 tl_list_purger_noop(void* pointer) {
    return true;
}