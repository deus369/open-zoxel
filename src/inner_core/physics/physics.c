#ifndef Zoxel_Physics
#define Zoxel_Physics

#include "physics2D/physics2D.c"
#include "physics3D/physics3D.c"

// #ifndef M_PI
// #define M_PI 3.141592653589793
// #endif

// Tags
ECS_DECLARE(Frictioned);

void PhysicsImport(ecs_world_t *world)
{
    ECS_MODULE(world, Physics);
    ECS_TAG_DEFINE(world, Frictioned);
    ECS_IMPORT(world, Physics2D);
    ECS_IMPORT(world, Physics3D);
}
#endif