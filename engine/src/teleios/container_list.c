#include "teleios/teleios.h"

TLList* tl_list_create(void) {
    TLDPUSH;

    TLList* list = tl_memory_alloc(TL_MEMORY_CONTAINER_LIST, sizeof(TLList));

    TLDRV(list);
}

void  tl_list_add(TLList* list, void* payload) {
    TLDPUSH;
    if (list == NULL) TLDWRE("List is NULl");

    TLListNode* node = tl_memory_alloc(TL_MEMORY_CONTAINER_LIST_ENTRY, sizeof(TLListNode));
    node->payload = payload;
    node->next = NULL;
    node->previous = NULL;

    if (list->length == 0) {
        list->head = node;
        list->tail = node;
        list->length++;
    
        TLDRE;
    }

    node->previous = list->tail;
    list->tail->next = node;
    list->tail = node;
    list->length++;

    TLDRE;
}

void tl_list_rem(TLList* list, void* payload) {
    TLDPUSH;
    if (list == NULL) TLDWRE("List is NULl");
 
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

            TLDRE;
            return;
        }

        node = node->next;
    }

    TLDWRE("Payload not found");
}

void  tl_list_destroy(TLList* list, b8(*purger)(void*)) {
    TLDPUSH;
    if (list == NULL) TLDWRE("List is NULl");

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
    TLDRE;
}

b8 tl_list_purger_noop(void* pointer) {
    return true;
}