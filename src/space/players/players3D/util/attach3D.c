void toggle_camera_perspective(ecs_world_t *world, ecs_entity_t character) {
    if (ecs_is_valid(world, character) && ecs_has(world, character, CameraLink)) {
        const CameraLink *cameraLink = ecs_get(world, character, CameraLink);
        if (ecs_is_valid(world, cameraLink->value) && cameraLink->value != 0) {
            float vox_scale = model_scale * 16;
            const LocalPosition3D *localPosition3D = ecs_get(world, cameraLink->value, LocalPosition3D);
            if (localPosition3D->value.z == vox_scale * 0.5f) {
                zox_set_only(cameraLink->value, LocalPosition3D, {{ 0, vox_scale * 2.2f, - vox_scale * 3.6f }})
                zox_set_only(cameraLink->value, LocalRotation3D, { quaternion_from_euler((float3) { 25, 180 * degreesToRadians, 0 }) })
            } else {
                zox_set_only(cameraLink->value, LocalPosition3D, {{ 0, vox_scale * 0.5f, vox_scale * 0.5f }})
                zox_set_only(cameraLink->value, LocalRotation3D, { quaternion_from_euler((float3) { 0, 180 * degreesToRadians, 0 }) })
            }
        }
    }
}

void detatch_from_character(ecs_world_t *world, ecs_entity_t player, ecs_entity_t camera, ecs_entity_t character) {
    zox_set_only(player, CharacterLink, { 0 })
    zox_add_tag(camera, EulerOverride)
    // zox_set_only(camera, FreeRoam, { 0 })
    zox_set_only(camera, ParentLink, { 0 })
    zox_remove(camera, FirstPersonCamera)
    zox_set_only(camera, CanFreeRoam, { 1 })
    zox_set_only(mouse_entity, MouseLock, { 0 })
    if (character != 0) {
        zox_remove(character, PlayerCharacter3D)
        zox_set_only(character, CameraLink, { 0 })
    }
    // fix caera rotation to be the same
    zoxel_log(" > [%lu] is detaching from character [%lu]\n", camera, character);
}

void attach_to_character(ecs_world_t *world, ecs_entity_t player, ecs_entity_t camera, ecs_entity_t character) {
    const float vox_scale = model_scale * 16;
    if (character == 0) detatch_from_character(world, player, camera, character);
    if (character == 0) return;
    // zoxel_log(" > attaching to character\n");
    // attach the camera with transform restraints
    const Rotation3D *rotation3D = ecs_get(world, character, Rotation3D);
    float3 local_camera_position = (float3) { 0, vox_scale * 2.2f, - vox_scale * 3.6f };
    zox_add_tag(camera, FirstPersonCamera)
    zox_set_only(camera, CanFreeRoam, { 0 })
    zox_set_only(camera, ParentLink, { character })
    zox_set_only(camera, Rotation3D, { rotation3D->value })
    zox_set_only(camera, LocalPosition3D, { local_camera_position })
    zox_set_only(camera, LocalRotation3D, { quaternion_from_euler((float3) { 25, 180 * degreesToRadians, 0 }) })
    // zox_remove(camera, FreeRoam)
    zox_remove(camera, EulerOverride)
    // character
    zox_add_tag(character, PlayerCharacter3D)
    zox_set_only(character, CameraLink, { camera })
    zox_set_only(character, DisableMovement, { 0 })
    // link character
    zox_set_only(player, CharacterLink, { character })
    // lock mouse since attached
    zox_set_only(mouse_entity, MouseLock, { 1 })
    // spawn_element3D(world, character);  // todo: also add this to character's UILinks
    zoxel_log(" > [%lu] is ataching to character [%lu]\n", camera, character);
}

// ecs_set(world, camera, LocalPosition3D, { 0, 0, vox_scale / 2.0f }); // zoxel_set
// ecs_set(world, camera, LocalPosition3D, { vox_scale / 2.0f, vox_scale + vox_scale / 2.0f, vox_scale });
// ecs_set(world, camera, LocalEuler3D, { float3_zero });
// zoxel_log("scale %f\n", vox_scale);
// const Position3D *position3D = ecs_get(world, character, Position3D);
// ecs_set(world, camera, Position3D, { position3D->value });
    
// this will just set camera to above character
/*zox_add_tag(camera, EulerOverride)
zox_remove(camera, FirstPersonCamera)
zox_set_only(camera, CanFreeRoam, { 1 })
const Position3D *character_position = ecs_get(world, character, Position3D);
zox_set_only(camera, Position3D, {{ character_position->value.x, character_position->value.y + vox_scale * 2.2f, character_position->value.z - vox_scale * 3.6f }})
return;*/
