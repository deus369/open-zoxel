//! Basic noise texture.
ecs_entity_t player_character3D_prefab;

void spawn_player_character3D_prefab(ecs_world_t *world) {
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, character3D_prefab);
    ecs_add_id(world, e, EcsPrefab);
    set_unique_entity_name(world, e, "player_character3D_prefab");
    zoxel_add_tag(world, e, PlayerCharacter);
    zoxel_add_tag(world, e, PlayerCharacter3D);
    zoxel_add(world, e, CameraLink);
    zoxel_set(world, e, DisableMovement, { 0 });
    ecs_remove(world, e, AnimateChunk);
    // printf("Has AnimateChunk? %b\n", ecs_has(world, e, AnimateChunk));
    ecs_defer_end(world);
    player_character3D_prefab = e;
    #ifdef zoxel_debug_prefabs
        zoxel_log("spawn_prefab player_character3D [%lu].\n", (long int) (e));
    #endif
}