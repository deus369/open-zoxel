#ifndef zox_mod_voxels
#define zox_mod_voxels

zox_component_entity(VoxLink)
#include "blocks/blocks.c"
#include "chunks/chunks.c"
#include "structures/structures.c"
#include "animations/voxels_animations.c"
#include "voxes/voxes.c"
#include "terrain/terrain.c"
#include "realm_voxels.c"

zox_begin_module(Voxels)
zox_define_component_entity(VoxLink)
zox_import_module(Blocks)
zox_import_module(Chunks)
zox_import_module(Structures)
zox_import_module(VoxelsAnimations)
zox_import_module(Voxes)
zox_import_module(Terrain)
zox_prefab_set(prefab_realm, VoxelLinks, { 0, NULL })
zox_prefab_set(prefab_realm, VoxelsDirty, { 0 })
zoxel_end_module(Voxels)

#endif
