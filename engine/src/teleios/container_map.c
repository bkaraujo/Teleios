#include "teleios/teleios.h"

TLMap* tl_map_create(b8 (*comparator)(void* first, void* second)) {
    TLDPUSH;

    if (comparator == NULL) TLDWRV("comparator is NULL", NULL);
    TLMap* map = tl_memory_alloc(TL_MEMORY_CONTAINER_MAP, sizeof(TLMap));
    map->comparator = comparator;

    TLDRV(map);
}

void tl_map_put(TLMap* map, void* key, void* payload) {
    TLDPUSH;

    if (map == NULL) TLDWRE("TLMap is NULL");
    if (key == NULL) TLDWRE("key is NULL");
    if (payload == NULL) TLDWRE("payload is NULL");

    if (map->length >= map->capacity) {
        u16 extended = (u16) map->capacity * 0.75f + 1;
        TLMapEntry* entries = tl_memory_alloc(TL_MEMORY_CONTAINER_NODE, sizeof(TLMapEntry) * extended);
        tl_memory_copy((void*)map->entries, sizeof(TLMapEntry) * map->capacity, (void*)entries);
        tl_memory_free(TL_MEMORY_CONTAINER_NODE, sizeof(TLMapEntry) * map->capacity, (void*) map->entries);
        map->entries = entries;
        map->capacity = extended;
    }

    for (u16 i = 0 ; i < map->length ; ++i) {
        TLMapEntry entry = map->entries[i];
        if (!map->comparator(entry.handle, key)) { continue; }
        tl_list_add(entry.values, payload);
        TLDRE;
    }
}

b8 tl_map_contains(TLMap* map, void* key) {
    TLDPUSH;

    if (map == NULL) TLDWRV("TLMap is NULL", false);
    if (key == NULL) TLDWRV("key is NULL", false);
    if (map->length == 0) TLDWRV("TLMap is empty", false);
    
    for (u16 i = 0 ; i < map->length ; ++i) {
        TLMapEntry entry = map->entries[i];
        if (!map->comparator(entry.handle, key)) { continue; }
        TLDRV(true);
    }

    TLDRV(false);
}

void tl_map_del(TLMap* map, void* key, void* payload) {
    TLDPUSH;

    if (map == NULL) TLDWRE("TLMap is NULL");
    if (key == NULL) TLDWRE("key is NULL");
    if (payload == NULL) TLDWRE("payload is NULL");
    if (map->length == 0) TLDWRE("TLMap is empty");
    
    for (u16 i = 0 ; i < map->length ; ++i) {
        TLMapEntry entry = map->entries[i];
        if (!map->comparator(entry.handle, key)) { continue; }
        tl_list_rem(map->entries[i].values, payload);
        break;
    }

    TLDWRE("Map key not found");
}

void tl_map_rem(TLMap* map, void* key) {
    TLDPUSH;

    if (map == NULL) TLDWRE("TLMap is NULL");
    if (key == NULL) TLDWRE("key is NULL");
    if (map->length == 0) TLDWRE("TLMap is empty");
    for (u16 i = 0 ; i < map->length ; ++i) {
        TLMapEntry entry = map->entries[i];
        if (!map->comparator(entry.handle, key)) { continue; }

        tl_list_destroy(map->entries[i].values, tl_list_purger_noop);
#if defined(TL_BUILD_ALPHA) || defined(TL_BUILD_BETA)
        // visual debug aid
        tl_memory_zero((void*) &map->entries[i], sizeof(TLMapEntry));
#endif
        if (map->length == 1) { map->length--; break; }
        if (i == map->length - 1) { map->length--; break; }

        tl_memory_copy(
            (void*) &map->entries[i + 1],
            sizeof(TLMapEntry) * (map->capacity - map->length),
            (void*) &map->entries[i]
        );

        break;
    }

    TLDWRE("Map key not found");
}

TLList* tl_map_values(TLMap* map, void* key) {
    TLDPUSH;

    if (map == NULL) TLDWRV("TLMap is NULL", NULL);
    if (key == NULL) TLDWRV("key is NULL", NULL);
    if (map->length == 0) { TLDPOP; return NULL; };

    for (u16 i = 0 ; i < map->length ; ++i) {
        TLMapEntry entry = map->entries[i];
        if (!map->comparator(entry.handle, key)) { continue; }
        TLDRV(entry.values);
    }

    TLDWRV("Map key not found", NULL);
}

void tl_map_destroy(TLMap* map, b8 (*purger)(void*)) {
    TLDPUSH;

    if (map == NULL) TLDWRE("TLMap is NULL");
    if (purger == NULL) TLDWRE("purger is NULL");

    if (map->capacity == 0) {
        tl_memory_free(TL_MEMORY_CONTAINER_MAP, sizeof(TLMap), (void*) map);
        TLDRE;
    }

    for(u16 i = 0 ; i < map->length ; ++i) {
        tl_list_destroy(map->entries[i].values, purger);
        map->entries[i].values = NULL;
    }

    tl_memory_free(TL_MEMORY_CONTAINER_NODE, sizeof(TLMapEntry) * map->capacity, (void*) map->entries);
    tl_memory_free(TL_MEMORY_CONTAINER_MAP, sizeof(TLMap), (void*) map);    

    TLDRE;
}