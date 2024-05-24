#if !defined(zox_mod_game) && defined(zox_mod_players)
#define zox_mod_game

// todo: refactor cameras, do they even need ScreenPosition? we should just have CanvasLinks, and each camera is linked to a bunch? idk
unsigned char boot_zoxel_game(ecs_world_t *world) {
#ifndef zox_beta
    game_name = "Zoxel";
#else
    game_name = "Zoxel b";
#endif
    const ecs_entity_t window = spawn_main_window(world);
    load_shaders(world);
    if (!headless) {
        char* icon_path = concat_file_path(resources_path, "textures"character_slash"game_icon.png");
        load_app_icon(zox_gett_value(window, SDLWindow), icon_path);
        free(icon_path);
        char* cursor_path = concat_file_path(resources_path, "textures"character_slash"cursor_02.png");
        set_sdl_cursor(cursor_path, int2_zero);
        free(cursor_path);
    } else {
        initialize_rendering(world);
    }
    // Realm,  players, skybox
    const ecs_entity_t realm = spawn_realm(world, prefab_realm);
    create_game_stats(world, realm);
    const ecs_entity_t game = spawn_game(world);
    zox_set(game, RealmLink, { realm })
    spawn_weather(world);
    if (!headless) spawn_music(world, instrument_piano); // _square
    spawn_players_cameras_canvases(world, game);
    // debug/testing
#ifdef zox_test_voxel_mesh
    test_animating_chunks(world, camera_begin_position, prefab_chunk_animating);
#endif
#ifdef zox_test_voxes
    test_voxes(world, camera_begin_position);
#endif
#ifdef zox_spawn_terrain_on_startup
    ecs_entity_t terrain = create_terrain(world);
    zox_set(terrain, RealmLink, { realm })
#endif
    test_steam_cloud();
    return EXIT_SUCCESS;
}

void ZoxGameImport(ecs_world_t *world) {
    zox_module(ZoxGame)
    boot_event = boot_zoxel_game;
    zox_game_type = zox_game_mode_3D;
    //menu_sky_color = (float3) { 29 / 255.0f, 53 / 255.0f, 47 / 255.0f };
    // menu_sky_bottom_color = (float3) { 30 / 255.0f, 25 / 255.0f, 21 / 255.0f };
    menu_sky_color = (float3) { 5 / 255.0f, 32 / 255.0f, 32  / 255.0f };
    menu_sky_bottom_color = (float3) { 5 / 255.0f, 32 / 255.0f, 32 / 255.0f };
}

#endif
