#ifndef Zoxel_Transforms
#define Zoxel_Transforms
//! The Transforms Module.

// Components
#include "Components/Position.c"
#include "Components/Rotation.c"
#include "Components/Scale.c"
#include "Components/UniformScale.c"
ZOXEL_COMPONENT(Euler, float3);
// Systems

void TransformsImport(ecs_world_t *world)
{
    ECS_MODULE(world, Transforms);
    ECS_COMPONENT_DEFINE(world, Position);
    ECS_COMPONENT_DEFINE(world, Rotation);
    ECS_COMPONENT_DEFINE(world, Scale);
    ECS_COMPONENT_DEFINE(world, UniformScale);
    ECS_COMPONENT_DEFINE(world, Euler);
    // Matrix System
}

#endif