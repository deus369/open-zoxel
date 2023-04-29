void spawn_connected_devices(ecs_world_t *world) {
    spawn_keyboard(world);
    spawn_mouse(world);
    spawn_sdl_gamepads();
    //if (joysticks_count > 0) {
        spawn_gamepad(world);
    //}
}

void input_extract_from_sdl(ecs_world_t *world, SDL_Event event, int2 screen_dimensions) {
    extract_keyboard(world, event);
    extract_mouse(world, event, screen_dimensions);
    // extract_gamepad(world, event);
    if (event.type == SDL_JOYDEVICEADDED) {
        if (joystick == NULL) {
            joystick = SDL_JoystickOpen(event.jdevice.which);
            int joystick_id = SDL_JoystickInstanceID(joystick);
            zoxel_log(" > gamepad [%d] has connected\n", joystick_id);
        }
    }
}

void reset_input_devices(ecs_world_t *world) {
    reset_gamepad(world, gamepad_entity);
    reset_keyboard(world, keyboard_entity);
    reset_mouse(world);
}