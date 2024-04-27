int begin_core(int argc, char* argv[]) {
    int didFail = process_arguments(argc, argv);
    if (didFail == EXIT_FAILURE) return EXIT_FAILURE;
    int cpu_core_count = SDL_GetCPUCount();
    begin_platforms(cpu_core_count);
    world = open_ecs(argc, argv, profiler, cpu_core_count);
    return EXIT_SUCCESS;
}

void update_core(ecs_world_t *world) {
    if (!headless) {
#ifdef zoxel_inputs
        device_reset_keyboard(world, keyboard_entity);
        device_reset_mouse(world, mouse_entity);
#endif
        update_sdl(world, main_app);
#ifdef zoxel_on_web
        update_web_canvas(world);   // handles resize event
#endif
    }
    clear_viewport();
    run_ecs(world);
    iterate_time();
    run_rendering();
#ifdef zoxel_log_frame_ms
    zox_log(" > frame time [%fms]\n", (float) (zox_delta_time * 1000.0f))
#endif
}
