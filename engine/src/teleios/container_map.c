#include "teleios/teleios.h"

TLMap* tl_map_create() {
    TLDPUSH;
    TLMap* map = tl_memory_alloc(TL_MEMORY_CONTAINER_MAP, sizeof(TLMap));
    TLDRV(map);
}

static TLOVERLOAD TLMapEntry* tl_map_entry(TLMap* map, u16 key) {
    TLDPUSH;

    if (map == NULL) TLDWRV("TLMap is NULL", NULL);
    
    TLListNode* node = map->head;
    while (node != NULL) {
        TLMapEntry* entry = node->payload;
        if (entry->handle.u16 == key) TLDRV(entry);
        node = node->next;
    }

    TLDRV(NULL);
}

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


TLOVERLOAD void tl_map_put(TLMap* map, u16 key, void* payload) {
    TLDPUSH;

    if (map == NULL) TLDWRE("TLMap is NULL");
    if (payload == NULL) TLDWRE("payload is NULL");
    
    TLMapEntry* entry = tl_map_entry(map, key);
    if (entry == NULL) {
        entry = tl_memory_alloc(TL_MEMORY_CONTAINER_NODE, sizeof(TLMapEntry));
        entry->handle.u16 = key;
        entry->values = tl_list_create();
        tl_list_add(map, entry);
    }

    tl_list_add(entry->values, payload);
    TLDRE;
}

TLOVERLOAD void tl_map_put(TLMap* map, TLUlid* key, void* payload) {
    TLDPUSH;

    if (map == NULL) TLDWRE("TLMap is NULL");
    if (key == NULL) TLDWRE("key is NULL");
    if (payload == NULL) TLDWRE("payload is NULL");
    
    TLMapEntry* entry = tl_map_entry(map, key);
    if (entry == NULL) {
        entry = tl_memory_alloc(TL_MEMORY_CONTAINER_NODE, sizeof(TLMapEntry));
        entry->handle.ulid = key;
        entry->values = tl_list_create();
        tl_list_add(map, entry);
    }

    tl_list_add(entry->values, payload);
    TLDRE;
}

#define TLMAPCONTAINS(map,key) {                            \
    TLDPUSH;                                                \
    b8 found = tl_map_entry(map, key) != NULL;              \
    TLDRV(found);                                           \
}

TLOVERLOAD b8 tl_map_contains(TLMap* map, u16 key) { TLMAPCONTAINS(map, key); }
TLOVERLOAD b8 tl_map_contains(TLMap* map, TLUlid* key) { TLMAPCONTAINS(map, key); }

#define TLMAPDEL(map,key,payload) {                             \
    TLDPUSH;                                                    \
    TLMapEntry* entry = tl_map_entry(map, key);                 \
    if (entry == NULL) TLDWRE("Key not found");                 \
    if (payload == NULL) TLDWRE("payload is NULL");             \
    tl_list_rem(entry->values, payload);                        \
    if (entry->values->length == 0) { tl_map_rem(map, key); }   \
    TLDRE;                                                      \
} 

TLOVERLOAD void tl_map_del(TLMap* map, u16 key, void* payload) { TLMAPDEL(map, key, payload); }
TLOVERLOAD void tl_map_del(TLMap* map, TLUlid* key, void* payload) { TLMAPDEL(map, key, payload); }

#define TLMAPREM(map,key) {                                 \
    TLDPUSH;                                                \
    TLMapEntry* entry = tl_map_entry(map, key);             \
    if (entry == NULL) TLDWRE("Key not found");             \
    tl_list_destroy(entry->values, tl_list_purger_noop);    \
    tl_list_rem(map, entry);                                \
    TLDRE;                                                  \
}

TLOVERLOAD void tl_map_rem(TLMap* map, u16 key) { TLMAPREM(map, key); }
TLOVERLOAD void tl_map_rem(TLMap* map, TLUlid* key) { TLMAPREM(map, key); }

#define TLMAPVALUES(map,key) {                              \
    TLDPUSH;                                                \
    TLMapEntry* entry = tl_map_entry(map, key);             \
    if (entry == NULL) TLDWRV("Key not found", NULL);       \
    TLDRV(entry->values);                                   \
}

TLOVERLOAD TLList* tl_map_values(TLMap* map, u16 key) { TLMAPVALUES(map, key); }
TLOVERLOAD TLList* tl_map_values(TLMap* map, TLUlid* key) { TLMAPVALUES(map, key); }

void tl_map_destroy(TLMap* map, b8 (*purger)(void*)) {
    TLDPUSH;

    if (map == NULL) TLDWRE("TLMap is NULL");
    if (purger == NULL) TLDWRE("purger is NULL");

    TLListNode* node = map->head;
    while (node != NULL) {
        TLMapEntry* entry = node->payload;
        tl_list_destroy(entry->values, purger);
        node = node->next;
    }

    tl_list_destroy(map, tl_list_purger_noop);
    TLDRE;
};