ecs_entity_t button_prefab;

ecs_entity_t spawn_prefab_button(ecs_world_t *world)
{
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_prefab(world, "button_prefab");
    #ifdef zoxel_debug_prefabs
    printf("spawn_prefab button [%lu].\n", (long int) (e));
    #endif
    zoxel_add(world, e, Children);
    zoxel_add_tag(world, e, Button);
    zoxel_add_tag(world, e, FrameTexture);
    add_ui_plus_components(world, e);
    zoxel_add_tag(world, e, Selectable);
    zoxel_set(world, e, SelectableState, { 0 });
    zoxel_add_tag(world, e, Clickable);
    zoxel_set(world, e, ClickableState, { 0 });
    button_prefab = e;
    ecs_defer_end(world);
    return e;
}

ecs_entity_t spawn_button(ecs_world_t *world, ecs_entity_t parent,
    int2 position, int2 size, float2 anchor, const char* text, int font_size, unsigned char layer,
    float2 parent_position2D, int2 parent_pixel_size)
{
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, button_prefab);
    float2 position2D = initialize_ui_components_2(world, e, parent, position, size, anchor, layer,
        parent_position2D, parent_pixel_size);
    Children children = { };
    initialize_memory_component_non_pointer(children, ecs_entity_t, 1);
    children.value[0] = spawn_zext(world, zext_prefab, e,
        (int2) { 0, 0 },
        (float2) { 0.5f, 0.5f },
        text, font_size, (layer + 1),
        position2D, size);
    ecs_set(world, e, Children, { children.length, children.value });
    ecs_defer_end(world);
    // printf("Texture ratio: %f - ScaleRatio: %f\n", (size.x / (float) size.y), (scaledSize2D.x / scaledSize2D.y));
    // printf("UI Element Spawned - Position [%ix%i] - Size [%ix%i] - Position(Real) [%fx%f] - ScaledSize [%fx%f]\n",
    //    position.x, position.y, size.x, size.y, position2D.x, position2D.y, scaledSize2D.x, scaledSize2D.y);
    #ifdef zoxel_debug_spawns
    printf("Spawned button [%lu]\n", (long int) e);
    #endif
    return e;
}