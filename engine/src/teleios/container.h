#ifndef TELEIOS_CONTAINER
#define TELEIOS_CONTAINER

#include "teleios/types.h"

b8 tl_list_purger_noop(void* pointer);

TLList* tl_list_create(void);
void tl_list_add(TLList* list, void* payload);
void tl_list_rem(TLList* list, void* payload);
void tl_list_destroy(TLList* list, b8 (*purger)(void*));

#endif // TELEIOS_CONTAINER
