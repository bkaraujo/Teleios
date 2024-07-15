#ifndef TELEIOS_CONTAINER
#define TELEIOS_CONTAINER

#include "teleios/types.h"

b8 tl_list_purger_noop(void* pointer);

TLList* tl_list_create(void);
void tl_list_add(TLList* list, void* payload);
void tl_list_rem(TLList* list, void* payload);
void tl_list_destroy(TLList* list, b8 (*purger)(void*));

TLMap* tl_map_create(b8 (*comparator)(void* first, void* second));
b8 tl_map_contains(TLMap* map, void* key);
void tl_map_put(TLMap* map, void* key, void* payload);
void tl_map_del(TLMap* map, void* key, void* payload);
void tl_map_rem(TLMap* map, void* key);
TLMap* tl_map_values(TLMap* map, void* key);
void tl_map_destroy(TLMap* map, b8 (*purger)(void*));

#endif // TELEIOS_CONTAINER
