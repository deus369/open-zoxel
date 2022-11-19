ecs_entity_t terrain_chunk_prefab;

ecs_entity_t spawn_prefab_terrain_chunk(ecs_world_t *world)
{
    int2 texture_size = { 16, 16 };
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, voxel_prefab);
    ecs_add_id(world, e, EcsPrefab);
    ecs_set_name(world, e, "prefab_terrain_chunk");
    zoxel_add_tag(world, e, TerrainChunk);
    zoxel_add(world, e, MeshUVs);
    add_gpu_uvs(world, e);
    add_texture(world, e, texture_size);
    add_noise_texture(world, e);
    zoxel_add_tag(world, e, DirtTexture);
    add_gpu_texture(world, e);
    ecs_defer_end(world);
    terrain_chunk_prefab = e;
    return e;
}

ecs_entity_t spawn_terrain_chunk(ecs_world_t *world, ecs_entity_t prefab,
    int3 chunk_position, float3 position, float scale)
{
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, prefab);
    ecs_set(world, e, ChunkPosition, { chunk_position });
    ecs_set(world, e, Position, { position });
    ecs_set(world, e, Scale1D, { scale });
    spawn_gpu_mesh(world, e);
    spawn_gpu_material(world, e, shader3D_textured);
    spawn_gpu_uvs(world, e);
    spawn_gpu_texture(world, e);
    ecs_defer_end(world);
    return e;
}