#include "teleios/teleios.h"

/** @brief List of all known entities */
static TLList* entities;
/** 
 * @brief Map of components by entity
 * @note Pointers are shaded with of_components maps
 */
static TLMap* of_entities;

/** 
 * @brief Map of components by type
 * @note Pointers are shaded with of_entities maps
 */
static TLMap* of_components;

// #####################################################################################################
//
//                                           C O M P O N E N T
//
// #####################################################################################################
static void* tl_ecs_component_allocator(u16 type, TLUlid* owner) {
    TLDPUSH;
    
    switch (type) {
        case TLNameComponentID: {
            TLNameComponent* component = (TLNameComponent*) tl_memory_alloc(TL_MEMORY_ECS_COMPONENT, sizeof(TLNameComponent));
            *(u16*)&component->type = type;
            component->owner = owner;

            TLDRV(component);
        } break;
    }

    TLDWRV("Unknown component type", NULL);
}

static void tl_ecs_component_dealocator(u16 type, void* component) {
    TLDPUSH;

    switch (type) {
        case TLNameComponentID: {
            tl_memory_free(TL_MEMORY_ECS_COMPONENT, sizeof(TLNameComponent), component);
        } break;
    }

    TLDRE;
}
// #####################################################################################################
//
//                                              E N T I T Y
//
// #####################################################################################################
TLUlid* tl_ecs_entity_create(void){
    TLDPUSH;
    
    TLUlid* entity = tl_ulid_generate();
    tl_list_add(entities, entity);
    TLDRV(entity);
}

b8 tl_ecs_entity_exists(TLUlid* entity){
    TLDPUSH;

    TLListNode* node = entities->head;
    while (node != NULL) {
        TLUlid* ulid = node->payload;
        if (tl_ulid_equals((TLUlid*) entity, ulid)) TLDRV(true);
        node = node->next;
    }

    TLDRV(false);
}

void tl_ecs_entity_attach(TLUlid* entity, u16 type){
    TLDPUSH;

    if(!tl_ecs_entity_exists(entity)) TLDWRE("Entity not found");
    
    void* component = tl_ecs_component_allocator(type, entity);
    if (component == NULL) TLDWRE("Failed to allocate component");

    tl_map_put(of_entities, entity, component);
    tl_map_put(of_components, type, component);

    TLDRE;
}

void tl_ecs_entity_detach(TLUlid* entity, u16 type){
    TLDPUSH;

    TLList* values = tl_map_values(of_entities, entity);
    if (values == NULL) TLDWRE("Entity not foud");

    TLListNode* node = values->head;
    while (node != NULL) {
        u16 component; tl_memory_copy(node->payload, sizeof(u16), (void*)&component);
        if (type == component) {
            tl_map_del(of_entities, entity, node->payload);
            tl_map_del(of_components, type, node->payload);
            tl_ecs_component_dealocator(type, node->payload);

            TLDRE;
        }

        node = node->next;
    }

    TLDWRE("Component not found");
}

void* tl_ecs_entity_component(TLUlid* entity, u16 type){
    TLDPUSH;

    TLList* values = tl_map_values(of_entities, entity);
    if (values == NULL) TLDWRV("Entity not foud", NULL);

    TLListNode* node = values->head;
    while (node != NULL) {
        u16 component; tl_memory_copy(node->payload, sizeof(u16), (void*)&component);
        if (type == component) TLDRV(node->payload);
        node = node->next;
    }

    TLDRV(NULL);
}

TLList* tl_ecs_entity_components(TLUlid* entity){
    TLDPUSH;
    TLList* values = tl_map_values(of_entities, entity);
    TLDRV(values);
}

void tl_ecs_entity_destroy(TLUlid* entity){
    TLDPUSH;

    // ========================================================
    // Remove the enitty from entities list.
    // ========================================================
    {
        TLListNode* node = entities->head;
        while (node != NULL) {
            TLUlid* ulid = node->payload;
            if (tl_ulid_equals(entity, ulid)) {
                tl_list_rem(entities, entity);
                break;
            }
            node = node->next;
        }

        if (node == NULL) TLDWRE("Entity not found");
    }
    // ========================================================
    // Remove component from of_entities map
    // Remove component from of_components map
    // Dealocate component memory
    // ========================================================
    {
        TLList* values = tl_map_values(of_entities, entity);
        TLListNode* node = values->head;
        while (node != NULL) {
            u16 component; tl_memory_copy(node->payload, sizeof(u16), (void*)&component);
            tl_map_del(of_components, component, node->payload);
            tl_ecs_component_dealocator(component, node->payload);
            node = node->next;
        }

        tl_map_rem(of_entities, entity);
    }
    // ========================================================
    // Dealocate entity handle
    // ========================================================
    tl_ulid_destroy(entity);

    TLDRE;
}
// #####################################################################################################
//
//                                           L I F E C Y C L E
//
// #####################################################################################################

b8 tl_ecs_initialize(void) {
    TLDPUSH;

    entities = tl_list_create();
    of_entities = tl_map_create();
    of_components = tl_map_create();

    TLDRV(true);
}

static b8 tl_ecs_entity_purger(void* pointer) {
    TLDPUSH;
    
    if (pointer != NULL) {
        tl_ecs_entity_destroy((TLUlid*) pointer);
    }

    TLDRV(true);
}

b8 tl_ecs_terminate(void) {
    TLDPUSH;
    
    tl_list_destroy(entities, tl_ecs_entity_purger); entities = NULL;
    tl_map_destroy(of_entities, tl_list_purger_noop); of_entities = NULL;
    tl_map_destroy(of_components, tl_list_purger_noop); of_components = NULL;

    TLDRV(true);
}