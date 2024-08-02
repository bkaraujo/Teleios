#include "teleios/teleios.h"

TLMap* tl_map_create(void) {
    TLDPUSH;
    TLMap* map = tl_memory_alloc(TL_MEMORY_CONTAINER_MAP, sizeof(TLMap));
    TLDRV(map);
}

TLOVERLOAD void tl_map_rem(TLMap* map, TLMapEntry* entry) {
    TLDPUSH;
    if (entry == NULL) TLDWRE("Key not found");
    
    tl_list_rem(map, entry); 
    tl_list_destroy(entry->values, tl_list_purger_noop);
    tl_memory_free(entry);
    TLDRE;
}

void tl_map_destroy(TLMap* map, b8 (*purger)(void*)) {
    TLDPUSH;

    if (map == NULL) TLDWRE("TLMap is NULL");
    if (purger == NULL) TLDWRE("purger is NULL");

    while (map->head != NULL) {
        TLMapEntry* entry = (TLMapEntry*)map->head->payload;

        tl_list_destroy(entry->values, purger);
        tl_memory_free(entry);

        tl_list_rem(map, map->head->payload);
    }
    
    tl_memory_free(map);
    TLDRE;
}