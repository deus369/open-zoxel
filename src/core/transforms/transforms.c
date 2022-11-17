#ifndef zoxel_transforms
#define zoxel_transforms

// shared components
//! Scales an entities transform in 1D.
zoxel_component(Scale1D, float);
// zoxel_entities_component(Children, ecs_entity_t);
zoxel_memory_component(Children, ecs_entity_t);
zoxel_component(ParentLink, ecs_entity_t);
// util
#include "util/hierarchy_util.c"
// Sub Modules
#include "transforms2D/transforms2D.c"
#include "transforms3D/transforms3D.c"

//! core transforms grabs dimension based sub modules.
/**
*   \todo Check out on_delete hooks for children deletions.
*/
void TransformsImport(ecs_world_t *world)
{
    ECS_MODULE(world, Transforms);
    ECS_COMPONENT_DEFINE(world, Scale1D);
    // zoxel_entities_component_define(world, Children);
    zoxel_memory_component_define(world, Children);
    ECS_COMPONENT_DEFINE(world, ParentLink);
    ECS_IMPORT(world, Transforms2D);
    ECS_IMPORT(world, Transforms3D);
}
#endif