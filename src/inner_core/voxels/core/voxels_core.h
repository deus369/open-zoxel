#ifndef zoxel_voxels_core
#define zoxel_voxels_core

const double noiseChunkAnimateSpeed = 0.5; // 1 / 8.0;
const float overall_voxel_scale = 2.0f;
const int chunk_length = 16;
const int dissapearChance = 92;
const float spawnRange = 0.96f;
const int3 chunk_size = { chunk_length, chunk_length, chunk_length };
const int3 terrain_chunk_size = { chunk_length, 8 * chunk_length, chunk_length };
const int terrain_rows = 12;
const float chunk_real_size = 1.0f;   // size achunk takes up
// tags
ECS_DECLARE(Vox);
ECS_DECLARE(NoiseChunk);
ECS_DECLARE(TerrainChunk);
// components
zoxel_component(ChunkDirty, unsigned char);
//! A simple chunk with an array of voxels.
zoxel_memory_component(Chunk, unsigned char);
//! A simple chunk with an array of voxels.
zoxel_component(ChunkSize, int3);
//! A state for generating chunks.
zoxel_component(GenerateChunk, unsigned char);
//! A local position of a chunk inside a Vox.
zoxel_component(ChunkPosition, int3);
//! A list to all chunks in a Vox model.
zoxel_memory_component(ChunkLinks, ecs_entity_t);
//! A list to all chunks in a Vox model.
zoxel_memory_component(ChunkNeighbors, ecs_entity_t);
//! A state for animating textures.
zoxel_component(AnimateChunk, double);
// util
#include "util/voxel_mesh_util.c"
#include "util/chunk.c"
#include "util/noise.c"
#include "util/chunk_build_util.c"
// prefabs
#include "prefabs/chunk.c"
#include "prefabs/voxel_chunk_mesh.c"
#include "prefabs/noise_chunk.c"
#include "prefabs/terrain_chunk.c"
// systems
zoxel_reset_system(GenerateChunkResetSystem, GenerateChunk);
zoxel_reset_system(ChunkDirtyResetSystem, ChunkDirty);
// #include "systems/generate_chunk_reset_system.c"
#include "systems/noise_chunk_system.c"
#include "systems/terrain_chunk_system.c"
#include "systems/animate_chunk_system.c"
#include "systems/chunk_build_system.c"
#include "systems/chunk_uvs_build_system.c"

int get_chunk_index(int i, int j, int terrain_rows)
{
    return (i + terrain_rows) * (terrain_rows + terrain_rows + 1) + (j + terrain_rows);
}

//! The voxels core Sub Module.
/**
*   \todo First use a Point Render system to render voxel data.
*   \todo Spawn an Oct Node of 1 Voxel, if open, create 8 sub chunk nodes. 
*   \todo Only push voxel data to the GPU.
*/
void VoxelsCoreImport(ecs_world_t *world)
{
    ECS_MODULE(world, VoxelsCore);
    ECS_TAG_DEFINE(world, Vox);
    ECS_TAG_DEFINE(world, NoiseChunk);
    ECS_TAG_DEFINE(world, TerrainChunk);
    ECS_COMPONENT_DEFINE(world, ChunkDirty);
    zoxel_memory_component_define(world, Chunk);
    ECS_COMPONENT_DEFINE(world, ChunkSize);
    ECS_COMPONENT_DEFINE(world, GenerateChunk);
    ECS_COMPONENT_DEFINE(world, ChunkPosition);
    zoxel_memory_component_define(world, ChunkLinks);
    zoxel_memory_component_define(world, ChunkNeighbors);
    ECS_COMPONENT_DEFINE(world, AnimateChunk);
    // filters
    // zoxel_filter(generateChunkQuery, world, [none] NoiseChunk, [in] GenerateChunk);
    // zoxel_filter(chunkDirtyQuery, world, [none] NoiseChunk, [in] EntityDirty);
    // systems
    ECS_SYSTEM_DEFINE(world, AnimateChunkSystem, EcsOnLoad, [out] AnimateChunk, [out] GenerateChunk);

    zoxel_filter(generateNoiseChunkQuery, world, [none] NoiseChunk, [in] GenerateChunk);
    zoxel_system_ctx(world, NoiseChunkSystem, EcsPostLoad, generateNoiseChunkQuery,
        [none] NoiseChunk, [out] ChunkDirty, [out] Chunk, [in] ChunkSize, [in] GenerateChunk);

    zoxel_filter(generateTerrainChunkQuery, world, [none] TerrainChunk, [in] GenerateChunk);
    zoxel_system_ctx(world, TerrainChunkSystem, EcsOnLoad, generateTerrainChunkQuery,
        [none] TerrainChunk, [out] ChunkDirty, [out] Chunk, [in] ChunkSize, [in] ChunkPosition, [in] GenerateChunk);

    // EcsOnValidate? why not working?
    zoxel_filter(generateChunkQuery, world, [in] GenerateChunk);
    zoxel_system_ctx(world, ChunkBuildSystem, EcsOnUpdate, generateChunkQuery,
        [in] ChunkDirty, [in] Chunk, [in] ChunkSize, [out] MeshIndicies, [out] MeshVertices, [out] MeshDirty, [none] !MeshUVs);
    zoxel_system_ctx(world, ChunkUVsBuildSystem, EcsOnUpdate, generateChunkQuery,
        [in] ChunkDirty, [in] Chunk, [in] ChunkSize, [in] ChunkNeighbors,
        [out] MeshIndicies, [out] MeshVertices, [out] MeshUVs, [out] MeshDirty);

    // zoxel_system_main_thread(world, GenerateChunkResetSystem, EcsPostUpdate, [out] GenerateChunk);
    zoxel_reset_system_define(GenerateChunkResetSystem, GenerateChunk);
    zoxel_reset_system_define(ChunkDirtyResetSystem, ChunkDirty);
    spawn_voxel_chunk_mesh_prefab(world);
    spawn_prefab_noise_chunk(world);
    spawn_prefab_terrain_chunk(world, terrain_chunk_size);
    #ifdef zoxel_test_voxels
    // test character voxels
    spawn_voxel_noise_chunk_mesh(world, prefab_noise_chunk, (float3) { 0, 0, 0 }, 0.15f);
    spawn_voxel_noise_chunk_mesh(world, prefab_noise_chunk, (float3) { 0, -spawnRange, 0 }, 0.05f);
    spawn_voxel_noise_chunk_mesh(world, prefab_noise_chunk, (float3) { 0, spawnRange, 0 }, 0.05f);
    spawn_voxel_noise_chunk_mesh(world, prefab_noise_chunk, (float3) { -spawnRange, 0, 0 }, 0.05f);
    spawn_voxel_noise_chunk_mesh(world, prefab_noise_chunk, (float3) { spawnRange, 0, 0 }, 0.05f);
    #endif
    #ifdef zoxel_test_voxels_terrain
    // todo: create a hashmap here
    ecs_entity_t chunks[(terrain_rows + terrain_rows + 1) * (terrain_rows + terrain_rows + 1)];
    for (int i = -terrain_rows; i <= terrain_rows; i++)
    {
        for (int j = -terrain_rows; j <= terrain_rows; j++)
        {
            // printf("%ix%i index is %i\n", i, j, get_chunk_index(i, j, terrain_rows));
            chunks[get_chunk_index(i, j, terrain_rows)] = spawn_terrain_chunk(world, terrain_chunk_prefab,
                (int3) { i, 0, j }, (float3) { i * chunk_real_size, 0, j * chunk_real_size }, 0.5f);
        }
    }
    // now for all of them, set their neighbors
    ecs_defer_begin(world);
    for (int i = -terrain_rows; i <= terrain_rows; i++)
    {
        for (int j = -terrain_rows; j <= terrain_rows; j++)
        {
            set_chunk_neighbors(world,
                chunks[get_chunk_index(i, j, terrain_rows)],
                i == -terrain_rows ? 0 : chunks[get_chunk_index(i - 1, j, terrain_rows)],
                i == terrain_rows ? 0 : chunks[get_chunk_index(i + 1, j, terrain_rows)],
                j == -terrain_rows ? 0 : chunks[get_chunk_index(i, j - 1, terrain_rows)],
                j == terrain_rows ? 0 : chunks[get_chunk_index(i, j + 1, terrain_rows)]);
        }
    }
    ecs_defer_end(world);
    #endif
}

// components
// #include "components/ChunkLinks.c"
// ECS_COMPONENT_DECLARE(ChunkLinks);
//! Stores voxels, in an oct tree.
/**
*   \todo Store Tree Nodes as entities. Each tree node can be open or closed.
*/
#endif