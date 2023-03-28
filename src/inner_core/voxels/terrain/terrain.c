#ifndef zoxel_voxels_terrain
#define zoxel_voxels_terrain

#ifndef WEB_BUILD
    #define terrain_spawn_distance 14   // for release, just incase
    #define inner_render_buffer 1       // 2, will lag too much if too high
    #define max_octree_depth 4
#else
    #define terrain_spawn_distance 10
    #define inner_render_buffer 1
    #define max_octree_depth 4
    #define voxels_disable_streaming
#endif
#define terrain_vertical 2
#define octree_min_height -1.995f // 0.005f
#define noise_positiver2 32000
#define terrain_amplifier 64.0
#define terrain_minus_amplifier 0.0
#ifdef voxel_octrees
    #define terrain_frequency 0.022216 // 0.026216
#else
    #define terrain_frequency 0.00216 // 0.004216
#endif
#define terrain_texture_resolution 16 // 32
const int3 terrain_chunk_size = { chunk_length, 8 * chunk_length, chunk_length };
float chunk_real_size = overall_voxel_scale / 2.0f; // 1.0f;   // size achunk takes up
const unsigned char terrain_min_height = 8;
const int terrain_octaves = 12;
const uint32_t terrain_seed = 32666;
const int2 chunk_texture_size = { terrain_texture_resolution, terrain_texture_resolution };
zoxel_declare_tag(TerrainWorld)
zoxel_declare_tag(TerrainChunk)
zoxel_declare_tag(ChunkTerrain)
zoxel_declare_tag(Streamer)
zoxel_component(StreamPoint, int3)                        //! A stream point in the terrain module.
#include "prefabs/terrain.c"
#include "prefabs/terrain_chunk.c"
#include "prefabs/terrain_chunk_octree.c"
#include "util/octree_tricks.c"
#include "util/static_octree_build.c"
#include "systems/terrain_chunk_system.c"
#include "systems/chunk_uvs_build_system.c"
#include "systems/stream_point_system.c"
#include "octree_systems/octree_terrain_chunk_system.c"
#include "octree_systems/octree_chunk_build_system.c"
#include "util/create_terrain.c"

zoxel_begin_module(Terrain)
zoxel_define_tag(TerrainWorld)
zoxel_define_tag(TerrainChunk)
zoxel_define_tag(ChunkTerrain)
zoxel_define_tag(Streamer)
zoxel_define_component(StreamPoint)
zoxel_filter(generateTerrainChunkQuery, world, [none] TerrainChunk, [in] GenerateChunk)
zoxel_filter(generateChunkQuery, world, [in] GenerateChunk)
zoxel_system_ctx(world, TerrainChunkSystem, EcsPostLoad, generateTerrainChunkQuery,
    [none] TerrainChunk, [out] ChunkDirty, [out] ChunkData, [in] ChunkSize, [in] ChunkPosition, [in] GenerateChunk)
zoxel_system_ctx(world, OctreeTerrainChunkSystem, EcsPostLoad, generateTerrainChunkQuery,
    [none] TerrainChunk, [in] ChunkPosition, [in] GenerateChunk, [out] ChunkDirty, [out] ChunkOctree)
zoxel_filter(terrain_chunks_query, world,
    [none] TerrainChunk, [in] ChunkPosition, [in] ChunkNeighbors, [out] ChunkDivision, [out] ChunkDirty)
zoxel_system_ctx_main_thread(world, StreamPointSystem, EcsOnUpdate, terrain_chunks_query,
    [none] Streamer, [in] Position3D, [out] StreamPoint)
zoxel_system_ctx(world, ChunkUVsBuildSystem, EcsOnUpdate, generateChunkQuery, [in] ChunkDirty, [in] ChunkData, [in] ChunkSize, [in] ChunkNeighbors,
    [out] MeshIndicies, [out] MeshVertices, [out] MeshUVs, [out] MeshDirty, [none] !MeshColors)
zoxel_system_ctx(world, OctreeChunkBuildSystem, EcsOnUpdate, generateChunkQuery,
    [in] ChunkDirty, [in] ChunkOctree, [in] ChunkDivision, [in] ChunkNeighbors,
    [out] MeshIndicies, [out] MeshVertices, [out] MeshUVs, [out] MeshDirty, [none] !MeshColors)
spawn_prefab_terrain(world);
spawn_prefab_terrain_chunk(world, terrain_chunk_size);
spawn_prefab_terrain_chunk_octree(world, terrain_chunk_size);
zoxel_end_module(Terrain)

// todo: move texture to terrain entity and not terrain chunks

/*  terrain_spawn_distance
    1,      // 0
    2,      // 1
    4,      // 2
    8,      // 3
    16,     // 4
    32,     // 5
    64,     // 6
    128     // 7
*/

#endif