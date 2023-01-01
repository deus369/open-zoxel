//! Basic Character 2D prefab.
ecs_entity_t character2D_prefab;

void spawn_prefab_character2D(ecs_world_t *world, int2 textureSize)
{
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_prefab(world, "character2D_prefab");
    #ifdef zoxel_transforms2D
    add_transform2Ds(world, e);
    #endif
    #ifdef zoxel_physics2D
    zoxel_add_tag(world, e, Frictioned);
    add_physics2D(world, e);
    #endif
    // Rendering2D
    add_seed(world, e, 444);
    zoxel_add(world, e, Brightness);
    add_dirty(world, e);
    add_texture(world, e, textureSize);
    add_animated_noise_texture(world, e);
    if (!headless)
    {
        add_gpu_material(world, e);
        add_gpu_texture(world, e);
    }
    ecs_defer_end(world);
    character2D_prefab = e;
    #ifdef zoxel_debug_prefabs
    zoxel_log("spawn_prefab character2D [%lu].\n", (long int) (e));
    #endif
}