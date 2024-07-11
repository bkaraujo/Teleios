#ifndef TELEIOS_ECS
#define TELEIOS_ECS

#include "teleios/types.h"

b8 tl_ecs_initialize(void);

#define TLNameComponentID 1

typedef struct {
    const u16 type;
    const char* owner;

    const char* name;
} TLNameComponent;

const char* tl_ecs_entity_create(void);
void tl_ecs_entity_attach(const char* entity, u16 component_id);
void tl_ecs_entity_detach(const char* entity, u16 component_id);
void* tl_ecs_entity_component(const char* entity, u16 component_id);
TLList* tl_ecs_entity_components(const char* entity);
void tl_ecs_entity_destroy(const char* entity);

TLList* tl_ecs_components(u16 component_id);

b8 tl_ecs_terminate(void);

#endif // TELEIOS_ECS