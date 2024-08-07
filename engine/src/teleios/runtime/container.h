#ifndef TELEIOS_CONTAINER
#define TELEIOS_CONTAINER

#include "teleios/runtime/types.h"

b8 tl_list_purger_noop(void* pointer);

TLList* tl_list_create(void);
void tl_list_add(TLList* list, void* payload);
void tl_list_rem(TLList* list, void* payload);
void tl_list_destroy(TLList* list, b8 (*purger)(void*));

TLMap* tl_map_create();

TLOVERLOAD b8 tl_map_contains(TLMap* map, u16 key);
TLOVERLOAD b8 tl_map_contains(TLMap* map, TLUlid* key);
TLOVERLOAD void tl_map_put(TLMap* map, u16 key, void* payload);
TLOVERLOAD void tl_map_put(TLMap* map, TLUlid* key, void* payload);
TLOVERLOAD void tl_map_del(TLMap* map, u16 key, void* payload);
TLOVERLOAD void tl_map_del(TLMap* map, TLUlid* key, void* payload);
TLOVERLOAD void tl_map_rem(TLMap* map, u16 key);
TLOVERLOAD void tl_map_rem(TLMap* map, TLUlid* key);
TLOVERLOAD void tl_map_rem(TLMap* map, TLMapEntry* entry);
TLOVERLOAD TLList* tl_map_values(TLMap* map, u16 key);
TLOVERLOAD TLList* tl_map_values(TLMap* map, TLUlid* key);

void tl_map_destroy(TLMap* map, b8 (*purger)(void*));

#endif // TELEIOS_CONTAINER
