ecs_entity_t prefab_base_camera;

ecs_entity_t spawn_camera_base_prefab(ecs_world_t *world) {
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_prefab(world, "");
    set_unique_entity_name(world, e, "prefab_base_camera");
    zox_add_tag(e, Camera);
    zox_add(e, Position3D);
    zox_add(e, Rotation3D);
    zox_set(e, ParentLink, { 0 });
    zox_add(e, LocalPosition3D);
    zox_set(e, LocalRotation3D, { quaternion_identity });
    zox_set(e, Euler, { { 0, 0, 0 } });
    zox_set(e, ProjectionMatrix, { float4x4_identity() });
    zox_set(e, ViewMatrix, { float4x4_identity() });
    zox_set(e, ScreenDimensions, { { 0, 0 } });
    zox_set(e, ScreenPosition, { { 0, 0 } });
    zox_set(e, FieldOfView, { camera_fov });
    zox_set(e, CameraNearDistance, { 0.01f });
    ecs_defer_end(world);
    prefab_base_camera = e;
    #ifdef zoxel_debug_prefabs
        zoxel_log(" + spawned prefab camera_base [%lu].\n", (long int) (e));
    #endif
    return e;
}

ecs_entity_t spawn_base_camera(ecs_world_t *world, float3 position, float4 rotation, int2 screen_dimensions, int2 screen_position) {
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, prefab_base_camera);
    set_unique_entity_name(world, e, "base_camera");
    ecs_set(world, e, Position3D, { position });
    ecs_set(world, e, Rotation3D, { rotation });
    ecs_set(world, e, ScreenDimensions, { screen_dimensions });
    ecs_set(world, e, ScreenPosition, { screen_position });
    ecs_defer_end(world);
    #ifdef zoxel_debug_spawns
        zoxel_log(" + spawned base camera [%lu]\n", (long int) e);
    #endif
    return e;
}