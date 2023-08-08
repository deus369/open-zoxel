int get_chunk_index(int i, int j, int rows) {
    return (i + rows + 1) * (rows + rows + 1) + (j + rows + 1);
}

// vertical_rows + vertical_rows +  - rows + rows + 1
int get_terrain_chunks_count(int rows, int slabs) {
    return (slabs + slabs + 1) * (rows + rows + 1) * (rows + rows + 1);
}

int get_chunk_index_2(int i, int j, int k, int rows, int vertical) {
    i += rows;
    j += vertical;
    k += rows;
    return i * (rows + rows + 1) + j * (rows + rows + 1) * (rows + rows + 1) + k;
}

ecs_entity_t create_terrain(ecs_world_t *world) {
    #ifdef zox_time_create_terrain
        begin_timing_absolute()
    #endif
    #ifdef zox_disable_terrain_octrees
        terrain_vertical = 0;
    #endif
    ecs_entity_t tilemap = spawn_tilemap(world);
    int chunks_total_length = get_terrain_chunks_count(terrain_spawn_distance, terrain_vertical);
    ecs_entity_t terrain_world = spawn_terrain(world, prefab_terrain, tilemap, float3_zero, 1.0f);  // todo link world to chunks and vice versa
    ecs_entity_t chunks[chunks_total_length];
    int3 chunk_positions[chunks_total_length];
    for (int i = -terrain_spawn_distance; i <= terrain_spawn_distance; i++) {
        for (int k = -terrain_spawn_distance; k <= terrain_spawn_distance; k++) {
            for (int j = -terrain_vertical; j <= terrain_vertical; j++) {
                int3 chunk_position = (int3) { i, j, k };
                int index = get_chunk_index_2(i, j, k, terrain_spawn_distance, terrain_vertical);
                #ifndef zox_disable_terrain_octrees
                    chunks[index] = spawn_terrain_chunk_octree(world, prefab_terrain_chunk_octree, terrain_world, chunk_position, (float3) { i * real_chunk_scale, j * real_chunk_scale, k * real_chunk_scale }, 0.5f);
                    chunk_positions[index] = chunk_position;
                #else
                    chunks[index] = spawn_terrain_chunk(world, prefab_terrain_chunk, chunk_position, (float3) { i * real_chunk_scale, 0, k * real_chunk_scale }, 0.5f);
                #endif
            }
        }
    }
    // now for all of them, set their neighbors
    for (int i = -terrain_spawn_distance; i <= terrain_spawn_distance; i++) {
        for (int k = -terrain_spawn_distance; k <= terrain_spawn_distance; k++) {
            for (int j = -terrain_vertical; j <= terrain_vertical; j++) {
                #ifndef zox_disable_terrain_octrees
                    set_chunk_neighbors_six_directions(world,
                        chunks[get_chunk_index_2(i, j, k, terrain_spawn_distance, terrain_vertical)],
                        i == -terrain_spawn_distance ? 0 : chunks[get_chunk_index_2(i - 1, j, k, terrain_spawn_distance, terrain_vertical)],
                        i == terrain_spawn_distance ? 0 : chunks[get_chunk_index_2(i + 1, j, k, terrain_spawn_distance, terrain_vertical)],
                        j == -terrain_vertical ? 0 : chunks[get_chunk_index_2(i, j - 1, k, terrain_spawn_distance, terrain_vertical)],
                        j == terrain_vertical ? 0 : chunks[get_chunk_index_2(i, j + 1, k, terrain_spawn_distance, terrain_vertical)],
                        k == -terrain_spawn_distance ? 0 : chunks[get_chunk_index_2(i, j, k - 1, terrain_spawn_distance, terrain_vertical)],
                        k == terrain_spawn_distance ? 0 : chunks[get_chunk_index_2(i, j, k + 1, terrain_spawn_distance, terrain_vertical)]);
                #else
                    set_chunk_neighbors(world,
                        chunks[get_chunk_index_2(i, j, k, terrain_spawn_distance, 0)],
                        i == -terrain_spawn_distance ? 0 : chunks[get_chunk_index_2(i - 1, j, k, terrain_spawn_distance, 0)],
                        i == terrain_spawn_distance ? 0 : chunks[get_chunk_index_2(i + 1, j, k, terrain_spawn_distance, 0)],
                        k == -terrain_spawn_distance ? 0 : chunks[get_chunk_index_2(i, j, k - 1, terrain_spawn_distance, 0)],
                        k == terrain_spawn_distance ? 0 : chunks[get_chunk_index_2(i, j, k + 1, terrain_spawn_distance, 0)]);
                #endif
            }
        }
    }
    ChunkLinks chunkLinks = { };
    chunkLinks.value = create_int3_hash_map(chunks_total_length);
    for (int i = 0; i < chunks_total_length; i++) int3_hash_map_add(chunkLinks.value, chunk_positions[i], chunks[i]);
    ecs_set(world, terrain_world, ChunkLinks, { chunkLinks.value });
    #ifdef zox_time_create_terrain
        end_timing_absolute("    - create_terrain")
    #endif
    return terrain_world;
}

void dispose_opengl_resources_terrain(ecs_world_t *world) {
    #ifndef voxels_terrain_multi_material
        const TilemapLink *tilemapLink = ecs_get(world, main_terrain, TilemapLink);
        dispose_material_resources(world, tilemapLink->value);
    #endif
    const ChunkLinks *chunkLinks = ecs_get(world, main_terrain, ChunkLinks);
    for (int i = 0; i < chunkLinks->value->size; i++) {
        int3_hash_map_pair* pair = chunkLinks->value->data[i];
        while (pair != NULL) {
            ecs_entity_t terrain_chunk = pair->value;
            dispose_mesh_resources(world, terrain_chunk);
            pair = pair->next;
        }
    }
}

void restore_opengl_resources_terrain(ecs_world_t *world) {
    #ifndef voxels_terrain_multi_material
        const TilemapLink *tilemapLink = ecs_get(world, main_terrain, TilemapLink);
        restore_material_resources(world, tilemapLink->value, shader3D_textured);
    #endif
    const ChunkLinks *chunkLinks = ecs_get(world, main_terrain, ChunkLinks);
    for (int i = 0; i < chunkLinks->value->size; i++) {
        int3_hash_map_pair* pair = chunkLinks->value->data[i];
        while (pair != NULL) {
            ecs_entity_t terrain_chunk = pair->value;
            restore_mesh_resources(world, terrain_chunk);
            pair = pair->next;
        }
    }
}

/*#ifdef voxels_terrain_multi_material
    restore_material_resources(world, terrain_chunk, shader3D_textured);
#endif*/