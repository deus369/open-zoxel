void toggle_ui2(ecs_world_t *world, const ecs_entity_t canvas, const ecs_entity_t e, ecs_entity_t (*spawn_ui)(ecs_world_t*, const ecs_entity_t)) {
    if (e == 0) {
        // zox_debug_children(world, canvas);
        (*spawn_ui)(world, canvas);
    } else {
        zox_delete(e)
    }
}

#define toggle_ui_with_tag(spawn_ui, tag) {\
    find_child_with_tag(canvas, tag, ui)\
    toggle_ui2(world, canvas, ui, spawn_ui);\
}

void toggle_ui(ecs_world_t *world, const ecs_entity_t canvas, ecs_entity_t *e, ecs_entity_t (*spawn_ui)(ecs_world_t*, const ecs_entity_t)) {
    if (*e == 0) {
        const ecs_entity_t e2 = (*spawn_ui)(world, canvas);
        *e = e2;
    } else {
        zox_delete(*e)
        *e = 0;
    }
}

// if player device mode is gamepad, select pause ui
#ifdef zoxel_include_players
// extern void raycaster_select_ui_mut(ecs_world_t *world, ecs_entity_t raycaster_entity, ecs_entity_t ui_entity);

void select_first_button(ecs_world_t *world, const ecs_entity_t element, const ecs_entity_t player) {
    if (!element) return;
    const DeviceMode *deviceMode = zox_get(player, DeviceMode)
    if (deviceMode->value == zox_device_mode_gamepad) {
        zox_log(" > selecting first button %lu\n", element)
        raycaster_select_element(world, player, element);
    }
}
#endif
