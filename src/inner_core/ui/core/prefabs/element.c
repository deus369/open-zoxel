ecs_entity_t prefab_element;

ecs_entity_t spawn_prefab_element(ecs_world_t *world) {
    zox_prefab()
    zox_prefab_name("prefab_element")
    zox_add_tag(e, FrameTexture)
    zox_prefab_set(e, FrameCorner, { 7 })
    zox_prefab_set(e, OutlineThickness, { 3 })
    zox_prefab_set(e, Color, {{ 255, 255, 255, 255 }}) // 66, 35, 25, 255
    zox_prefab_set(e, SelectState, { 0 })
    add_ui_plus_components(world, e);
    prefab_element = e;
    return e;
}

ecs_entity_t spawn_element(ecs_world_t *world, const ecs_entity_t canvas, const ecs_entity_t parent, int2 pixel_position, const int2 pixel_size, const float2 anchor, const unsigned char layer, const color element_color, const int2 parent_pixel_position_global, const int2 parent_pixel_size) {
    const int2 canvas_size = zox_get_value(canvas, PixelSize)
    const int2 pixel_position_global = get_element_pixel_position_global(parent_pixel_position_global, parent_pixel_size, pixel_position, anchor);
    const float2 position2D = get_element_position(pixel_position_global, canvas_size);
    anchor_element_position2D(&pixel_position, anchor, pixel_size);
    zox_instance(prefab_element)
    zox_name("element")
    initialize_element(world, e, parent, canvas, pixel_position, pixel_size, pixel_size, anchor, layer, position2D, pixel_position_global);
    zox_set(e, Color, { element_color })
    return e;
}

ecs_entity_t spawn_element_on_canvas(ecs_world_t *world, const ecs_entity_t canvas, const int2 pixel_position, const int2 pixel_size, const float2 anchor) {
    const int2 canvas_size = zox_get_value(canvas, PixelSize)
    return spawn_element(world, canvas, canvas, pixel_position, pixel_size, anchor, 0, (color) { 66, 35, 25, 255 }, int2_half(canvas_size), canvas_size);
}
