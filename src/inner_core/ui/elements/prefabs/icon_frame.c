// a small square frame ui with an icon in it
// used for game - action/skill/stat - uis
ecs_entity_t spawn_prefab_icon_frame(ecs_world_t *world, const ecs_entity_t prefab) {
    zox_prefab_child(prefab)
    zox_prefab_name("prefab_icon_frame")
    zox_add_tag(e, IconFrame)
    zox_prefab_set(e, OutlineColor, {{ 255, 0, 0, 255 }})
    zox_prefab_add(e, Children)
    return e;
}

ecs_entity_t spawn_icon_frame(ecs_world_t *world, SpawnIconFrame *data) {
    int2 position = data->element.position; // anchor our position
    const int2 position_in_canvas = get_element_pixel_position_global(data->parent.position, data->parent.size, data->element.position, data->element.anchor);
    const float2 real_position = get_element_position(position_in_canvas, data->canvas.size);
    anchor_element_position2D(&position, data->element.anchor, data->element.size);
    zox_instance(data->prefab)
    // zox_name("icon_frame")
    initialize_element(world, e, data->parent.e, data->canvas.e, position, data->element.size, data->element.size, data->element.anchor, data->element.layer, real_position, position_in_canvas);
    zox_set(e, Color, { data->icon_frame.fill_color })
    zox_set(e, OutlineColor, { data->icon_frame.outline_color })
    SpawnIcon spawnIcon = {
        .prefab = prefab_icon,
        .icon = data->icon,
        .canvas = data->canvas,
        .parent = {
            .e = e,
            .position = position_in_canvas,
            .size = data->element.size
        },
        .element = {
            .size = (int2) { data->element.size.x - 8, data->element.size.y - 8 },
            .anchor = float2_half,
            .layer = data->element.layer + 1,
        }
    };
    zox_get_mutt(e, Children, children)
    // initialize_memory_component(Children, children, ecs_entity_t, 1);
    // children->value[0] = spawn_icon(world, &spawnIcon);
    add_to_Children(children, spawn_icon(world, &spawnIcon));
    zox_modified(e, Children)
    return e;
}
