#include "teleios/teleios.h"

typedef struct {
    const char* handle;
    TLList* components;
} TLEntity;
static TLList* entities;    // All known entities

typedef struct {
    const u16 identity;
    TLList* entries;
} TLComponent;
static TLList* components;  // All known components

b8 tl_ecs_initialize(void) {
    TLDIAGNOSTICS_PUSH;
    entities = tl_list_create();
    components = tl_list_create();

    TLDIAGNOSTICS_POP;
    return true;
}

static TLEntity* tl_get_entity(const char* handle) {
    TLDIAGNOSTICS_PUSH;
    TLListNode* nEntity = entities->head;
    while (nEntity != NULL) {
        // Set the entt pointer to the desired entity
        TLEntity* entt = nEntity->payload;
        if (!tl_string_equal(entt->handle, handle)) {
            nEntity = nEntity->next;
            continue;
        }

        TLDIAGNOSTICS_POP;
        return entt;
    }
    
    TLDIAGNOSTICS_POP;
    return NULL;
}

const char* tl_ecs_entity_create() {
    TLDIAGNOSTICS_PUSH;

    TLEntity* entity = tl_memory_alloc(TL_MEMORY_ECS_ENTITY, sizeof(TLEntity));
    
    entity->handle = tl_ulid_generate();
    entity->components = tl_list_create();
    tl_list_add(entities, entity);

    TLDIAGNOSTICS_POP;
    return entity->handle;
}

void tl_ecs_entity_attach(const char* entity, u16 component_id) {
    TLDIAGNOSTICS_PUSH;

    TLEntity* entt = tl_get_entity(entity);
    if (entt == NULL) { TLWARN("Entity not found: %s", entity); TLDIAGNOSTICS_POP; return; }

    void* component = NULL;
    switch (component_id) {
        default: TLFATAL("Unknown component %u", component_id);
        case TLNameComponentID: {
            component = tl_memory_alloc(TL_MEMORY_ECS_COMPONENT, sizeof(TLNameComponent));
            
            TLNameComponent* nc = (TLNameComponent*) component;
            nc->owner = entity;
            *((u16*)&nc->type) = component_id;
        } break;
    }

    tl_list_add(entt->components, component);
    TLComponent* tlc = NULL;
    // ==============================================
    // Locate the appropriate TLComponent
    // ==============================================
    if (components->length == 0) {
        tlc = tl_memory_alloc(TL_MEMORY_ECS_COMPONENT, sizeof(TLComponent));
        *((u16*)&tlc->identity) = component_id;
        tlc->entries = tl_list_create();
    } else {
        TLListNode* node = components->head;
        while (node != NULL) {
            tlc = node->payload;
            if (tlc->identity != component_id) {
                node = node->next;
                continue;
            }

            break;
        }

        tlc = tl_memory_alloc(TL_MEMORY_ECS_COMPONENT, sizeof(TLComponent));
        *((u16*)&tlc->identity) = component_id;
        tlc->entries = tl_list_create();
    }

    tl_list_add(tlc->entries, component);
    TLDIAGNOSTICS_POP;
}

void tl_ecs_entity_detach(const char* entity, u16 component_id) {
    TLDIAGNOSTICS_PUSH;
    TLDIAGNOSTICS_POP;
}

void* tl_ecs_entity_component(const char* entity, u16 component_id) {
    TLDIAGNOSTICS_PUSH;

    TLEntity* entt = tl_get_entity(entity);
    if (entt == NULL) { TLDIAGNOSTICS_POP; return NULL; }

    TLList* list = entt->components;
    if (list->length == 0) { TLDIAGNOSTICS_POP; return NULL; }

    TLListNode* node = list->head;
    while (node != NULL) {
        void* component = node->payload;
        u16 cid; tl_memory_copy((void*)component, sizeof(u16), (void*)&cid);
        if (cid != component_id) {
            node = node->next;
            continue;
        }

        // return the found component
        TLDIAGNOSTICS_POP;
        return component;
    }

    TLDIAGNOSTICS_POP;
    return NULL;
}

TLList* tl_ecs_entity_components(const char* entity) {
    TLDIAGNOSTICS_PUSH;
    TLDIAGNOSTICS_POP;
    return NULL;
}

void tl_ecs_entity_destroy(const char* entity) {
    TLDIAGNOSTICS_PUSH;

    TLListNode* node = entities->head;
    while (node != NULL) {
        TLEntity* payload = node->payload;
        if (payload->handle != entity) {
            node = node->next;
            continue;
        }

        node = NULL;
        tl_list_rem(entities, payload);
        tl_list_destroy(payload->components, tl_list_purger_noop);
        tl_ulid_destroy(payload->handle);
        tl_memory_free(TL_MEMORY_ECS_ENTITY, sizeof(TLEntity), payload);
    }

    TLDIAGNOSTICS_POP;
}

TLList* tl_ecs_components(u16 component_id) {
    TLDIAGNOSTICS_PUSH;
    TLDIAGNOSTICS_POP;
    return NULL;
}

b8 tl_ecs_terminate(void) {
    TLDIAGNOSTICS_PUSH;
    TLDIAGNOSTICS_POP;
    return true;
}