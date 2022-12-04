#ifndef zoxel_outer_core
#define zoxel_outer_core

// --- Outer core ---
#include "cubes/cubes.c"
#include "ai/ai.c"
#include "particles/particles.c"
#include "ui/ui.c"

void update_outer_core()
{
    iterate_fps_time();
}

void OuterCoreImport(ecs_world_t *world)
{
    ECS_MODULE(world, OuterCore);
    ECS_IMPORT(world, Cubes);
    ECS_IMPORT(world, AI);
    ECS_IMPORT(world, Particles);
    ECS_IMPORT(world, UI);
}
#endif