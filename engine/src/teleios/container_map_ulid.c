#include "teleios/teleios.h"

static TLOVERLOAD TLMapEntry* tl_map_entry(TLMap* map, TLUlid* key) {
    TLDPUSH;

    if (map == NULL) TLDWRV("TLMap is NULL", NULL);
    if (key == NULL) TLDWRV("key is NULL", NULL);
    
    TLListNode* node = map->head;
    while (node != NULL) {
        TLMapEntry* entry = node->payload;
        if (tl_ulid_equals(entry->handle.ulid, key)) TLDRV(entry);
        node = node->next;
    }

    TLDRV(NULL);
}

TLOVERLOAD void tl_map_put(TLMap* map, TLUlid* key, void* payload) {
    TLDPUSH;

    if (map == NULL) TLDWRE("TLMap is NULL");
    if (key == NULL) TLDWRE("key is NULL");
    if (payload == NULL) TLDWRE("payload is NULL");
    
    TLMapEntry* entry = tl_map_entry(map, key);
    if (entry == NULL) {
        entry = tl_memory_alloc(TL_MEMORY_CONTAINER_MAP_ENTRY, sizeof(TLMapEntry));
        entry->handle.ulid = key;
        entry->values = tl_list_create();
        tl_list_add(map, entry);
    }

    tl_list_add(entry->values, payload);
    TLDRE;
}

TLOVERLOAD b8 tl_map_contains(TLMap* map, TLUlid* key) {
    TLDPUSH;
    b8 found = tl_map_entry(map, key) != NULL;
    TLDRV(found);
}

TLOVERLOAD void tl_map_del(TLMap* map, TLUlid* key, void* payload) {
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


TLOVERLOAD void tl_map_rem(TLMap* map, TLUlid* key) {
    TLDPUSH;
    TLMapEntry* entry = tl_map_entry(map, key);
    tl_map_rem(map, entry);
    TLDRE;
}

TLOVERLOAD TLList* tl_map_values(TLMap* map, TLUlid* key) {
    TLDPUSH;
    TLMapEntry* entry = tl_map_entry(map, key);
    if (entry == NULL) TLDRV(NULL);
    TLDRV(entry->values);
}