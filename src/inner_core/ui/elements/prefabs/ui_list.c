ecs_entity_t prefab_ui_list;

ecs_entity_t spawn_prefab_ui_list(ecs_world_t *world) {
    ecs_defer_begin(world);
    zox_prefab_child(window_prefab)
    zox_name("prefab_ui_list")
    // zox_add_tag(e, UIList)
    ecs_defer_end(world);
    prefab_ui_list = e;
    return e;
}

typedef struct {
    const char* text;
} text_group;

ecs_entity_t spawn_ui_list(ecs_world_t *world, ecs_entity_t prefab, const char *header_label,
    int list_count, const text_group labels[], const ClickEvent events[],
    int2 position, float2 anchor, unsigned char is_close_button) {
    int2 canvas_size = ecs_get(world, main_canvas, PixelSize)->value;
    float ui_scale = default_ui_scale;
    int font_size = 28;
    int header_margins = 12;
    int2 window_size = { 260, 60 + 40 * list_count };
    window_size.x *= ui_scale;
    window_size.y *= ui_scale;
    font_size *= ui_scale;
    header_margins *= ui_scale;
    int2 button_padding = (int2) { (int) (font_size * 0.6f), (int) (font_size * 0.3f) };
    int header_height = (font_size + header_margins - 1);
    int2 header_position = (int2) { 0, header_height / 2 };
    anchor_position2D(&position, window_size, anchor, header_height);
    // ecs_defer_begin(world);
    zox_instance(prefab)
    zox_name("ui_list")
    float2 position2D = initialize_ui_components(world, e, main_canvas, position, window_size, anchor, 0, canvas_size);
    Children children = { };
    initialize_memory_component_non_pointer(children, ecs_entity_t, (1 + list_count))
    children.value[0] = spawn_header(world, e, header_position,
        (int2) { window_size.x, font_size + header_margins }, (float2) { 0.5f, 1.0f },
        header_label, font_size, header_margins, 1, position2D, window_size, is_close_button, canvas_size);
    for (int i = 0; i < list_count; i++) {
        int2 label_position = (int2) { 0, font_size * 2 - i * font_size * 2 };
        if (list_count % 2 == 0) label_position.y -= font_size;
        const char* label = labels[i].text;
        int array_index = 1 + i;
        children.value[array_index] = spawn_button(world, e, label_position, button_padding,
            (float2) { 0.5f, 0.5f }, label, font_size, 1, position2D, window_size, canvas_size);
        zox_set(children.value[array_index], ClickEvent, { events[i].value })
    }
    ecs_set(world, e, Children, { children.length, children.value });
    if (list_count > 0) select_first_button(world, children.value[1]);
    // ecs_defer_end(world);
    #ifdef zoxel_debug_spawns
        zoxel_log(" > spawned ui list [%lu]\n", (long int) e);
    #endif
    return e;
}