#ifndef zoxel_transforms2D
#define zoxel_transforms2D

// components
//! A simple 2D position component.
zoxel_component(Position2D, float2);
//! Rotates in 2D!
zoxel_component(Rotation2D, float);
//! Scales in 2D! - put this in core or TransformShared
zoxel_component(Scale1D, float);
zoxel_component(Scale2D, float2);
// util
#include "util/transform2D_util.c"

//! Used for 2D transforms positioning.
void Transforms2DImport(ecs_world_t *world)
{
    ECS_MODULE(world, Transforms2D);
    ECS_COMPONENT_DEFINE(world, Position2D);
    ECS_COMPONENT_DEFINE(world, Rotation2D);
    ECS_COMPONENT_DEFINE(world, Scale1D);
    ECS_COMPONENT_DEFINE(world, Scale2D);
}
#endif