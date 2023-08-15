const double gamepad_rotate_multiplier = 0.08; // 0.09;
const double mouse_rotate_multiplier = 0.0032; // 0.008;
const float min_mouse_delta3 = 0.01f;
const float min_mouse_delta2 = 24.0f;
const float max_mouse_delta3 = 60.0f;
const float max_mouse_delta4 = 120.0f;
const float max_mouse_delta2 = 200.0f;
const double max_rotate_speed = 0.2; //  0.23;

float4 mouse_delta_to_rotation(float deltaX, float deltaY) {
    float4 output = quaternion_identity;
    output.x = deltaX;
    output.y = deltaY;
    // quaternion_normalize(&output);
    return output;
}

void Player3DRotateSystem(ecs_iter_t *it) {
    ecs_world_t *world = it->world;
    /*ecs_query_t *playerCharacterQuery = it->ctx;
    ecs_iter_t playerCharacterIterator = ecs_query_iter(it->world, playerCharacterQuery);
    ecs_query_next(&playerCharacterIterator);
    if (playerCharacterIterator.count == 0) return;*/
    const DeviceLinks *deviceLinks = ecs_field(it, DeviceLinks, 2);
    const CharacterLink *characterLinks = ecs_field(it, CharacterLink, 3);
    /*Alpha3D *alpha3Ds = ecs_field(&playerCharacterIterator, Alpha3D, 2);
    const Omega3D *omega3Ds = ecs_field(&playerCharacterIterator, Omega3D, 3);
    const DisableMovement *disableMovements = ecs_field(&playerCharacterIterator, DisableMovement, 4);*/
    for (int i = 0; i < it->count; i++) {
        const CharacterLink *characterLink = &characterLinks[i];
        if (characterLink->value == 0) continue;
        const DisableMovement *disableMovement = ecs_get(world, characterLink->value, DisableMovement);
        // if (disableMovement->value) zoxel_log(" > movement is disabled\n");
        if (disableMovement->value) continue;
        const DeviceLinks *deviceLinks2 = &deviceLinks[i];
        float2 euler = { 0, 0 };
        for (int j = 0; j < deviceLinks2->length; j++) {
            ecs_entity_t device_entity = deviceLinks2->value[j];
            if (ecs_has(world, device_entity, Mouse)) {
                const Mouse *mouse = ecs_get(world, device_entity, Mouse);
                float mouse_delta = mouse->delta.x;
                // if (mouse_delta != 0) zoxel_log(" > mouse_delta: %f\n", mouse_delta);
                if (mouse_delta < -max_mouse_delta2) mouse_delta = -max_mouse_delta;
                else if (mouse_delta > max_mouse_delta2) mouse_delta = max_mouse_delta;
                if (float_abs(mouse_delta) >= max_mouse_delta3) mouse_delta *= 1.2f;
                if (float_abs(mouse_delta) >= max_mouse_delta4) mouse_delta *= 1.4f;
                else if (float_abs(mouse_delta) <= min_mouse_delta3) mouse_delta = 0;
                if (mouse_delta != 0) {
                    if (mouse_delta > 0 && mouse_delta < min_mouse_delta2) mouse_delta = min_mouse_delta2;
                    else if (mouse_delta < 0 && mouse_delta > -min_mouse_delta2) mouse_delta = -min_mouse_delta2;
                }
                // if (mouse_delta != 0) zoxel_log("     > mouse_delta: %f\n", mouse_delta);
                euler.y = - mouse_delta * mouse_rotate_multiplier;
            } else if (ecs_has(world, device_entity, Gamepad)) {
                const Gamepad *gamepad = ecs_get(world, device_entity, Gamepad);
                if (gamepad->right_stick.value.x < -joystick_buffer) {
                    euler.y = gamepad->right_stick.value.x * gamepad_rotate_multiplier;
                } else if (gamepad->right_stick.value.x > joystick_buffer) {
                    euler.y = gamepad->right_stick.value.x * gamepad_rotate_multiplier;
                }
            }
        }
        if (euler.x == 0 && euler.y == 0) continue;
        const Omega3D *omega3D = ecs_get(world, characterLink->value, Omega3D);
        if ((euler.y > 0 && quaternion_to_euler_y(omega3D->value) < max_rotate_speed) || (euler.y < 0 && quaternion_to_euler_y(omega3D->value) > -max_rotate_speed)) {
            float4 quaternion = mouse_delta_to_rotation(euler.x, euler.y);
            Alpha3D *alpha3D = ecs_get_mut(world, characterLink->value, Alpha3D);
            quaternion_rotate_quaternion_p(&alpha3D->value, quaternion);
            ecs_modified(world, characterLink->value, Alpha3D);
        }
        /*for (int j = 0; j < playerCharacterIterator.count; j++) {
            const DisableMovement *disableMovement = &disableMovements[j];
            // Alpha3D *alpha3D = &alpha3Ds[j];
            if (disableMovement->value) continue;
            const Omega3D *omega3D = &omega3Ds[j];
            if ((euler.y > 0 && quaternion_to_euler_y(omega3D->value) < max_rotate_speed) || (euler.y < 0 && quaternion_to_euler_y(omega3D->value) > -max_rotate_speed)) {
                Alpha3D *alpha3D = &alpha3Ds[j];
                quaternion_rotate_quaternion_p(&alpha3D->value, quaternion);
            }
        }*/
    }
} zox_declare_system(Player3DRotateSystem)