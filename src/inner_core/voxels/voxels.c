#ifndef zoxel_voxels
#define zoxel_voxels

#define model_scale (1 / 32.0f) // 0.0006f         // 0.015f
// zoxel_component_includes
zoxel_declare_tag(Chunk)
zoxel_declare_tag(NoiseChunk)
zoxel_component(ChunkPosition, int3)
zoxel_component(VoxelPosition, int3)
zoxel_component(VoxLink, ecs_entity_t)
zoxel_component(ChunkLink, ecs_entity_t)
zoxel_hashmap_component(ChunkLinks, int3)
// zoxel_module_includes
#include "core/voxels_core.c"
#include "terrain/terrain.c"
#include "animations/voxels_animations.c"
#include "voxes/voxes.c"

zoxel_begin_module(Voxels)
// zoxel_component_defines
zoxel_define_tag(Chunk)
zoxel_define_tag(NoiseChunk)
zoxel_define_component(ChunkPosition)
zoxel_define_component(VoxelPosition)
zoxel_define_component(VoxLink)
zoxel_define_component(ChunkLink)
zoxel_define_hashmap_component(ChunkLinks)
// zoxel_module_defines
zoxel_import_module(VoxelsCore)
zoxel_import_module(Terrain)
zoxel_import_module(VoxelsAnimations)
zoxel_import_module(Vox)
zoxel_end_module(Voxels)

#endif