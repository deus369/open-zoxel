ecs_entity_t prefab_terrain_chunk;

ecs_entity_t spawn_prefab_terrain_chunk(ecs_world_t *world, int3 size) {
    ecs_defer_begin(world);
    // ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, prefab_chunk);
    ecs_entity_t e = ecs_clone(world, 0, prefab_chunk, 1);
    ecs_add_id(world, e, EcsPrefab);
    // set_unique_entity_name(world, e, "prefab_terrain_chunk");
    // ecs_set_name(world, e, "prefab_terrain_chunk");
    zox_set(e, ChunkSize, { size });
    zox_add_tag(e, TerrainChunk);
    if (!headless) {
        zox_add(e, MeshUVs);
    }
    add_gpu_uvs(world, e);
    #ifdef voxels_terrain_multi_material
        add_gpu_material(world, e);
        zox_add_tag(e, DirtTexture);
        add_texture(world, e, chunk_texture_size, 1);
        add_noise_texture(world, e);
        add_gpu_texture(world, e);
    #else
        ecs_remove(world, e, MaterialGPULink);
    #endif
    ecs_defer_end(world);
    prefab_terrain_chunk = e;
    return e;
}

ecs_entity_t spawn_terrain_chunk(ecs_world_t *world, ecs_entity_t prefab, int3 chunk_position, float3 position, float scale) {
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, prefab);
    set_unique_entity_name(world, e, "terrain_chunk");
    ecs_set(world, e, ChunkPosition, { chunk_position });
    ecs_set(world, e, Position3D, { position });
    ecs_set(world, e, Scale1D, { scale });
    if (!headless) {
        spawn_gpu_mesh(world, e);
        spawn_gpu_uvs(world, e);
        #ifdef voxels_terrain_multi_material
            spawn_gpu_material(world, e, shader3D_textured);
            spawn_gpu_texture(world, e);
        #endif
    }
    ecs_defer_end(world);
    return e;
}

void set_chunk_neighbors(ecs_world_t *world, ecs_entity_t e,
    ecs_entity_t chunk_left, ecs_entity_t chunk_right, ecs_entity_t chunk_back, ecs_entity_t chunk_front) {
    ChunkNeighbors chunkNeighbors = { };
    initialize_memory_component_non_pointer(chunkNeighbors, ecs_entity_t, 4);
    chunkNeighbors.value[0] = chunk_left;
    chunkNeighbors.value[1] = chunk_right;
    chunkNeighbors.value[2] = chunk_back;
    chunkNeighbors.value[3] = chunk_front;
    ecs_set(world, e, ChunkNeighbors, { chunkNeighbors.length, chunkNeighbors.value });
}

void set_chunk_neighbors_six_directions(ecs_world_t *world, ecs_entity_t e,
    ecs_entity_t chunk_left, ecs_entity_t chunk_right,
    ecs_entity_t chunk_down, ecs_entity_t chunk_up,
    ecs_entity_t chunk_back, ecs_entity_t chunk_front) {
    ChunkNeighbors chunkNeighbors = { };
    initialize_memory_component_non_pointer(chunkNeighbors, ecs_entity_t, 6);
    chunkNeighbors.value[0] = chunk_left;
    chunkNeighbors.value[1] = chunk_right;
    chunkNeighbors.value[2] = chunk_down;
    chunkNeighbors.value[3] = chunk_up;
    chunkNeighbors.value[4] = chunk_back;
    chunkNeighbors.value[5] = chunk_front;
    ecs_set(world, e, ChunkNeighbors, { chunkNeighbors.length, chunkNeighbors.value });
}
