ecs_entity_t spawn_prefab_block_vox(ecs_world_t *world, const ecs_entity_t prefab) {
    zox_prefab_child(prefab)
    zox_prefab_name("prefab_block_vox")
    zox_prefab_set(e, CloneVox, { 0})
    zox_prefab_set(e, CloneVoxLink, { 0 })
    // zox_prefab_set(e, ChunkDirty, { chunk_dirty_state_generated })
    zox_add_tag(e, BlockVox)
    zox_prefab_set(e, BlockIndex, { 0 })
    zox_prefab_set(e, RenderLod, { 255 }) // 1
    // BlockLink to original voxel meta data
    zox_prefab_set(e, Position3D, { float3_zero })
    zox_prefab_set(e, Rotation3D, { float4_identity })
    zox_prefab_set(e, VoxScale, { vox_model_scale * 1 })
    // for debug
    zox_prefab_set(e, Bounds3D, { (float3) { 0.5f, 0.5f, 0.5f } })
#ifdef zoxel_debug_bounds
    prefab_add_cube_lines(world, e, color_rgb_white, 1);
#else
    prefab_add_cube_lines(world, e, color_rgb_white, 0);
#endif
    return e;
}

ecs_entity_t spawn_block_vox(ecs_world_t *world, const SpawnBlockVox *data) {
    zox_instance(data->prefab)
    zox_name("block_vox")
    zox_set(e, BlockIndex, { data->block_index })
    zox_set(e, Position3D, { data->position_real })
    zox_set(e, RenderLod, { data->render_lod })
    zox_set(e, RenderDisabled, { data->render_disabled })
    spawn_gpu_mesh(world, e);
    spawn_gpu_colors(world, e);
    // todo: clone vox over system
    // clone_vox_data(world, e, data->vox, 1 + max_octree_depth - min_block_vox_lod);
    zox_set(e, CloneVox, { 1 })
    zox_set(e, CloneVoxLink, { data->vox })
    return e;
}
