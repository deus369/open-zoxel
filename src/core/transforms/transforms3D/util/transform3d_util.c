//! Add transform2D components onto an entity.
void add_transform3Ds(ecs_world_t *world, ecs_entity_t e) {
    zox_set(e, Position3D, { float3_zero })
    zox_set(e, Rotation3D, { float4_identity() })
    zox_set(e, Scale1D, { 1 })
    zox_set(e, LastPosition3D, { float3_zero })
}