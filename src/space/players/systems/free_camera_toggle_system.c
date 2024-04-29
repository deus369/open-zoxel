void FreeCameraToggleSystem(ecs_iter_t *it) {
    zox_iter_world()
    zox_field_in(DeviceLinks, deviceLinkss, 1)
    zox_field_in(CameraLink, cameraLinks, 2)
    zox_field_in(GameLink, gameLinks, 3)
    for (int i = 0; i < it->count; i++) {
        zox_field_i_in(GameLink, gameLinks, gameLink)
        if (!gameLink->value) continue;
        const GameState *gameState = zox_get(gameLink->value, GameState)
        if (gameState->value != zox_game_playing) continue;
        zox_field_i_in(CameraLink, cameraLinks, cameraLink)
        if (cameraLink->value == 0) continue;
        const CanFreeRoam *canFreeRoam = zox_get(cameraLink->value, CanFreeRoam)
        if (canFreeRoam->value == 0) continue;
        zox_field_i_in(DeviceLinks, deviceLinkss, deviceLinks)
        unsigned char is_triggered = 0;
        ecs_entity_t mouse_entity = 0;
        for (int j = 0; j < deviceLinks->length; j++) {
            const ecs_entity_t device_entity = deviceLinks->value[j];
            if (zox_has(device_entity, Mouse)) {
                const Mouse *mouse = zox_get(device_entity, Mouse)
                if (mouse->right.pressed_this_frame) is_triggered = 1;
                mouse_entity = device_entity;
            }
        }
        if (is_triggered && mouse_entity != 0) {
            const MouseLock *mouseLock = zox_get(mouse_entity, MouseLock)
            unsigned char new_value = !mouseLock->value;
            // zoxel_log(" > free camera toggled [%s]\n", new_value ? "on" : "off");
            zox_set(mouse_entity, MouseLock, { new_value })
            zox_set(cameraLink->value, FreeRoam, { new_value })
        }
    }
} zox_declare_system(FreeCameraToggleSystem)
