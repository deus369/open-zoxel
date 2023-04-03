ecs_entity_t prefab_free_camera;

ecs_entity_t spawn_prefab_free_camera(ecs_world_t *world) {
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, prefab_base_camera);
    ecs_add_id(world, e, EcsPrefab);
    zoxel_set(world, e, FreeRoam, { 0 });
    ecs_defer_end(world);
    prefab_free_camera = e;
    #ifdef zoxel_debug_prefabs
        zoxel_log("spawn_prefab free_camera [%lu].\n", (long int) (e));
    #endif
    return e;
}

ecs_entity_t spawn_free_camera(ecs_world_t *world, float3 position, float4 rotation,
    int2 screen_dimensions, int2 screen_position) {
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, prefab_free_camera);
    set_unique_entity_name(world, e, "free_camera");
    ecs_set(world, e, Position3D, { position });
    ecs_set(world, e, Rotation3D, { rotation });
    ecs_set(world, e, ScreenDimensions, { screen_dimensions });
    ecs_set(world, e, ScreenPosition, { screen_position });
    #ifndef zoxel_quaternion_camera
        ecs_set(world, e, Euler, { quaternion_to_euler(rotation) });
        zoxel_add_tag(world, e, EulerOverride);
        // adds a limit, - half pi to + half pi, 0.8 is around half that
        zoxel_set(world, e, EulerLimitX, { { -0.8f, 0.8f } });
    #endif
    ecs_defer_end(world);
    #ifdef zoxel_debug_spawns
        zoxel_log("Spawned Free Camera [%lu]\n", (long int) e);
    #endif
    return e;
}

    /*if (test_quaternion_math(rotation)) {
        // ecs_set(world, e, Euler, { quaternion_to_euler(rotation) });
    }*/
    // zoxel_set(world, e, FreeRoam, { 0 });