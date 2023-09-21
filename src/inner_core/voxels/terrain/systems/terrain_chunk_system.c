// #define zoxel_time_terrain_chunk_system
#define noise_positiver 3200

void generate_chunk_terrain(ChunkData* chunk, const int3 chunkSize, const int3 chunkPosition) {
    int3 local_position;
    int3 global_position;
    int3 chunk_position_offset = voxel_chunk_position_xz(chunkPosition, chunkSize);
    for (local_position.x = 0; local_position.x < chunkSize.x; local_position.x++) {
        for (local_position.z = 0; local_position.z < chunkSize.z; local_position.z++) {
            local_position.y = 0;
            global_position = int3_add(local_position, chunk_position_offset);
             float2 noise_point = float2_from_int2((int2) { global_position.x + noise_positiver, global_position.z + noise_positiver });
            int terrain_height2 = terrain_min_height +
                int_floor(terrain_boost + terrain_amplifier *
                perlin_terrain(noise_point.x, noise_point.y, terrain_frequency, terrain_seed, terrain_octaves));
            if (terrain_height2 < terrain_min_height) terrain_height2 = terrain_min_height;
            for (local_position.y = 0; local_position.y < chunkSize.y; local_position.y++) {
                chunk->value[int3_array_index(local_position, chunkSize)] = (local_position.y <= terrain_height2) ? 1 : 0;
            }
        }
    }
}

void TerrainChunkSystem(ecs_iter_t *it) {
    if (!ecs_query_changed(it->ctx, NULL)) return;
    #ifdef zoxel_time_terrain_chunk_system
        begin_timing()
    #endif
    ChunkDirty *chunkDirtys = ecs_field(it, ChunkDirty, 2);
    ChunkData *chunks = ecs_field(it, ChunkData, 3);
    const ChunkSize *chunkSizes = ecs_field(it, ChunkSize, 4);
    const ChunkPosition *chunkPositions = ecs_field(it, ChunkPosition, 5);
    GenerateChunk *generateChunks = ecs_field(it, GenerateChunk, 6);
    for (int i = 0; i < it->count; i++) {
        GenerateChunk *generateChunk = &generateChunks[i];
        if (generateChunk->value == 0) continue;
        ChunkDirty *chunkDirty = &chunkDirtys[i];
        if (chunkDirty->value != 0) continue;
        ChunkData *chunk = &chunks[i];
        const ChunkSize *chunkSize = &chunkSizes[i];
        const ChunkPosition *chunkPosition = &chunkPositions[i];
        re_initialize_memory_component(chunk, unsigned char, chunkSize->value.x * chunkSize->value.y * chunkSize->value.z);
        generate_chunk_terrain(chunk, chunkSize->value, chunkPosition->value);
        // printf("Terrain ChunkData Generated: [%lu] \n", (long int) it->entities[i]);
        #ifdef zoxel_time_terrain_chunk_system
            did_do_timing()
        #endif
        generateChunk->value = 0;
        chunkDirty->value = 1;
    }
    #ifdef zoxel_time_terrain_chunk_system
        end_timing("TerrainChunkSystem")
    #endif
}
zox_declare_system(TerrainChunkSystem)