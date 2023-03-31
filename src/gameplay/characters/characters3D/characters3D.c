#ifndef zoxel_characters3D
#define zoxel_characters3D

#define spawn_characters3D_count 32 // 64
#define spawn_many_radius 2  // 0.5
#define spawn_many_y 2.1f   // 0.7f
#define model_scale 0.0008f    // 0.015f
// const int spawn_characters3D_count = 1; // 64;
zoxel_declare_tag(Character3D)
zoxel_component(Character3DLink, ecs_entity_t)
#include "prefabs/character3D.c"
#include "fun/spawn_many.c"

zoxel_begin_module(Characters3D)
zoxel_define_tag(Character3D)
zoxel_define_component(Character3DLink)
spawn_prefab_character3D(world);
#ifdef zoxel_test_character3Ds
spawn_many_characters3D(world);
#endif
zoxel_end_module(Characters3D)

#endif