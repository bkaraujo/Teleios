#include "teleios/teleios.h"

static TLOVERLOAD TLMapEntry* tl_map_entry(TLMap* map, u16 key) {
    TLDPUSH;

    if (map == NULL) TLDWRV("TLMap is NULL", NULL);
    
    TLListNode* node = map->head;
    while (node != NULL) {
        TLMapEntry* entry = (TLMapEntry*) node->payload;
        if (entry->handle.u16 == key) TLDRV(entry);
        node = node->next;
    }

    TLDRV(NULL);
}

TLOVERLOAD void tl_map_put(TLMap* map, u16 key, void* payload) {
    TLDPUSH;

    if (map == NULL) TLDWRE("TLMap is NULL");
    if (payload == NULL) TLDWRE("payload is NULL");
    
    TLMapEntry* entry = tl_map_entry(map, key);
    if (entry == NULL) {
        entry = tl_memory_alloc(TL_MEMORY_CONTAINER_MAP_ENTRY, sizeof(TLMapEntry));
        entry->handle.u16 = key;
        entry->values = tl_list_create();
        tl_list_add(map, entry);
    }

    tl_list_add(entry->values, payload);
    TLDRE;
}

TLOVERLOAD b8 tl_map_contains(TLMap* map, u16 key) {
    TLDPUSH;
    b8 found = tl_map_entry(map, key) != NULL;
    TLDRV(found);
}

TLOVERLOAD void tl_map_del(TLMap* map, u16 key, void* payload) {
    TLDPUSH;
    if (payload == NULL) TLDWRE("payload is NULL");
    
    // ###############################################
    // Find the map entry
    // Remove the payload from the entry
    // ###############################################
    TLMapEntry* entry = tl_map_entry(map, key);
    if (entry == NULL) TLDWRE("Key not found");
    tl_list_rem(entry->values, payload);

    // ###############################################
    // Remove the entry if its empty
    // ###############################################
    if (entry->values->length == 0) { 
        tl_map_rem(map, entry);
    }

    TLDRE;
}

TLOVERLOAD void tl_map_rem(TLMap* map, u16 key) {
    TLDPUSH;
    TLMapEntry* entry = tl_map_entry(map, key);
    tl_map_rem(map, entry);
    TLDRE;
}

TLOVERLOAD TLList* tl_map_values(TLMap* map, u16 key) {
    TLDPUSH;
    TLMapEntry* entry = tl_map_entry(map, key);
    if (entry == NULL) TLDRV(NULL);
    TLDRV(entry->values);
}