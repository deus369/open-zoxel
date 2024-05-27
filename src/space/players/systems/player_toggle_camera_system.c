void PlayerToggleCameraSystem(ecs_iter_t *it) {
    zox_iter_world()
    zox_field_in(DeviceLinks, deviceLinkss, 1)
    zox_field_in(CharacterLink, characterLinks, 2)
    zox_field_in(GameLink, gameLinks, 3)
    for (int i = 0; i < it->count; i++) {
        zox_field_i_in(GameLink, gameLinks, gameLink)
        if (!gameLink->value) continue;
        const GameState *gameState = zox_get(gameLink->value, GameState)
        if (gameState->value != zox_game_playing) continue;
        zox_field_i_in(CharacterLink, characterLinks, characterLink)
        if (!characterLink->value) continue;
        if (zox_has(characterLink->value, DisableMovement)) {
            const DisableMovement *disableMovement = zox_get(characterLink->value, DisableMovement)
            if (disableMovement->value) continue;
        }
        unsigned char is_toggle_camera = 0;
        zox_field_i_in(DeviceLinks, deviceLinkss, deviceLinks)
        for (int j = 0; j < deviceLinks->length; j++) {
            const ecs_entity_t device_entity = deviceLinks->value[j];
            if (zox_has(device_entity, Mouse)) {
                const Mouse *mouse = zox_get(device_entity, Mouse)
                if (mouse->middle.pressed_this_frame) is_toggle_camera = 1;
            } else if (zox_has(device_entity, Keyboard)) {
                const Keyboard *keyboard = zox_get(device_entity, Keyboard)
                if (keyboard->y.pressed_this_frame) is_toggle_camera = 1;
            } else if (zox_has(device_entity, Gamepad)) {
                const Children *zevices = zox_get(device_entity, Children)
                for (int k = 0; k < zevices->length; k++) {
                    ecs_entity_t zevice_entity = zevices->value[k];
                    if (zox_has(zevice_entity, ZeviceButton)) {
                        const ZeviceDisabled *zeviceDisabled = zox_get(zevice_entity, ZeviceDisabled)
                        if (zeviceDisabled->value) continue;
                        const DeviceButtonType *deviceButtonType = zox_get(zevice_entity, DeviceButtonType)
                        if (deviceButtonType->value == zox_device_button_right_stick_push) {
                            const ZeviceButton *zeviceButton = zox_get(zevice_entity, ZeviceButton)
                            if (devices_get_pressed_this_frame(zeviceButton->value)) is_toggle_camera = 1;
                        }
                    }
                }
            }
        }
        if (is_toggle_camera) {
            toggle_camera_mode(world);
            // hack to disable crosshair for different camera modes
            zox_set(local_crosshair, RenderDisabled, { camera_mode != zox_camera_mode_first_person })
        }
    }
} zox_declare_system(PlayerToggleCameraSystem)
