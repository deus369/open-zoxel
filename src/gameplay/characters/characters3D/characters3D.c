#ifndef zoxel_characters3D
#define zoxel_characters3D

// zoxel_component_includes
zoxel_declare_tag(Character3D)
zoxel_component(Character3DLink, ecs_entity_t)
#include "settings/settings.c"
// zoxel_prefab_includes
#include "prefabs/character3D.c"
// zoxel_fun_includes
#include "fun/spawn_many.c"
// zoxel_module_includes
#include "terrain/characters3D_terrain.c"

zoxel_begin_module(Characters3D)
set_character_settings();
// zoxel_component_defines
zoxel_define_tag(Character3D)
zoxel_define_component(Character3DLink)
// zoxel_prefab_defnies
spawn_prefab_character3D(world);
// zoxel_module_defines
zoxel_import_module(Characters3DTerrain)
zoxel_end_module(Characters3D)

#endif