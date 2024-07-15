#ifndef TELEIOS_ECS
#define TELEIOS_ECS

#include "teleios/types.h"

b8 tl_ecs_initialize(void);

#define TLNameComponentID 1

typedef struct {
    const u16 type;
    const TLUlid* owner;

    const char* name;
} TLNameComponent;

TLUlid* tl_ecs_entity_create(void);
b8 tl_ecs_entity_exists(const TLUlid* entity);
void tl_ecs_entity_attach(const TLUlid* entity, const u16 type);
void tl_ecs_entity_detach(const TLUlid* entity, const u16 type);
void* tl_ecs_entity_component(const TLUlid* entity, const u16 type);
TLList* tl_ecs_entity_components(const TLUlid* entity);
void tl_ecs_entity_destroy(TLUlid* entity);

b8 tl_ecs_terminate(void);

#endif // TELEIOS_ECS