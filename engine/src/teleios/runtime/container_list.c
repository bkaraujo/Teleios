#include "teleios/runtime/teleios.h"

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
 
    if (list->head->payload == payload) {
        TLListNode* node = list->head;

        if (list->length == 1) {
            list->head = NULL;
            list->tail = NULL;
        } else {
            list->head = node->next;
            list->head->previous = NULL;
        }
        
        list->length--;
        tl_memory_free(node);

        TLDRE;
    }
    
    if (list->tail->payload == payload) {
        TLListNode* node = list->head;

        if (list->length == 1) {
            list->head = NULL;
            list->tail = NULL;
        } else {
            list->tail = node->previous;
            list->tail->next = NULL;
        }
        
        list->length--;
        tl_memory_free(node);

        TLDRE;
    }

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

            tl_memory_free(node);
            TLDRE;
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
            if (node->payload != NULL) purger(node->payload);
            tl_memory_free(node);
            node = next;
        }
    }
 
    tl_memory_free(list);
    TLDRE;
}

b8 tl_list_purger_noop(void* pointer) {
    return true;
}