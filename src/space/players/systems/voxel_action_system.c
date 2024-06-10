// left click - destroy
void VoxelActionASystem(ecs_iter_t *it) {
#ifdef zox_disable_raycasts3D
    return;
#endif
    zox_iter_world()
    // zox_field_in(CameraLink, cameraLinks, 1)
    // zox_field_in(VoxLink, voxLinks, 2)
    zox_field_in(RaycastVoxelData, raycastVoxelDatas, 1)
    zox_field_out(TriggerActionA, triggerActionAs, 2)
    for (int i = 0; i < it->count; i++) {
        zox_field_o(TriggerActionA, triggerActionAs, triggerActionA)
        if (!triggerActionA->value) continue;
        zox_field_i(RaycastVoxelData, raycastVoxelDatas, raycastVoxelData)
        raycast_action(world, raycastVoxelData, 0, 2);
        triggerActionA->value = 0;
    }
} zox_declare_system(VoxelActionASystem)

// right click = place
void VoxelActionBSystem(ecs_iter_t *it) {
#ifdef zox_disable_raycasts3D
    return;
#endif
    zox_iter_world()
    zox_field_in(RaycastVoxelData, raycastVoxelDatas, 1)
    zox_field_out(TriggerActionB, triggerActionBs, 2)
    for (int i = 0; i < it->count; i++) {
        zox_field_o(TriggerActionB, triggerActionBs, triggerActionB)
        if (!triggerActionB->value) continue;
        zox_field_i(RaycastVoxelData, raycastVoxelDatas, raycastVoxelData)
        raycast_action(world, raycastVoxelData, 1, 1);
        triggerActionB->value = 0;
    }
} zox_declare_system(VoxelActionBSystem)