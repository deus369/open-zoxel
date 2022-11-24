//! Spawns a basic zigel for a zext.
ecs_entity_t spawn_zigel(ecs_world_t *world, ecs_entity_t zext, unsigned char index,
    int2 position, int2 size, float2 anchor, unsigned char layer,
    float2 parent_position2D, int2 parent_pixel_size)
{
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, zigel_prefab);
    zoxel_set(world, e, ZigelIndex, { index });
    initialize_ui_components_2(world, e, zext, position, size, anchor, layer,
        parent_position2D, parent_pixel_size);
    ecs_defer_end(world);
    return e;
}