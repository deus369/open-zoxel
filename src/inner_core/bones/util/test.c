void make_test_skeleton(ecs_world_t *world, const ecs_entity_t e) {
    zox_get_muter(e, Children, children)
    zox_add_tag(e, SkeletonRenderer)
    zox_add_tag(e, Skeleton)
    zox_prefab_add(e, BoneLinks)
    zox_prefab_add(e, BoneIndexes)
    zox_get_muter(e, BoneLinks, boneLinks)
    // add bone one
    const ecs_entity_t bone = spawn_bone(world, prefab_bone, float3_zero, float3_zero);
    zox_set(bone, ParentLink, { e })
    add_to_Children(children, bone);
    add_to_BoneLinks(boneLinks, bone);
    // add head bone
    zox_get_muter(bone, Children, bone_children)
    const ecs_entity_t bone_child = spawn_bone(world, prefab_bone, float3_zero, test_spawn_bone_position);
    zox_set(bone_child, ParentLink, { bone })
    // add osccilation to head bone
    zox_set(bone_child, OscillateStartPosition3D, { test_bone_position })
    zox_add_tag(bone_child, OscillatePosition3D)
    add_to_Children(bone_children, bone_child);
    add_to_BoneLinks(boneLinks, bone_child);
}