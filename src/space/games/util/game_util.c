extern void attach_to_character(ecs_world_t *world, ecs_entity_t player, ecs_entity_t camera, ecs_entity_t character);
extern void button_event_pause_game(ecs_world_t *world, ecs_entity_t trigger_entity);
extern ecs_entity_t local_character3D;
unsigned char game_rule_attach_to_character = 1;
// do I still need render_camera_matrix? - yes - used to insert matrix in each camera system run
ecs_entity_t game_ui = 0;

void dispose_in_game_ui(ecs_world_t *world) {
    if (game_ui) {
        zox_delete(game_ui)
        game_ui = 0;
    }
}

void spawn_in_game_ui(ecs_world_t *world) {    // spawn game uis
    const DeviceMode *deviceMode = ecs_get(world, main_player, DeviceMode);
    unsigned char is_touch = deviceMode->value == zox_device_mode_touchscreen;
    #ifdef zoxel_mouse_emulate_touch
        is_touch = 1;
    #endif
    if (is_touch) {
        const unsigned char pause_button_size = 80;
        const ClickEvent on_pause_ui = (ClickEvent) { &button_event_pause_game };
        game_ui = spawn_button_on_canvas(world, main_canvas, (int2) { pause_button_size, pause_button_size }, (byte2) { 8, 4 }, (color) { 77, 32, 44, 255 }, "x", pause_button_size, float2_zero, on_pause_ui);   
    }
}

extern void detatch_from_character(ecs_world_t *world, ecs_entity_t player, ecs_entity_t camera, ecs_entity_t character);

void end_game(ecs_world_t *world) {
    zoxel_log(" > game state => [playing] to [main_menu]\n");
    zox_delete(pause_ui)
    // detatch character
    ecs_entity_t main_camera = main_cameras[0]; // get player camera link instead
    ecs_entity_t character = 0;
    if (camera_follow_mode == zox_camera_follow_mode_attach) character = ecs_get(world, main_camera, ParentLink)->value;
    else if (camera_follow_mode == zox_camera_follow_mode_follow_xz) character = ecs_get(world, main_camera, CameraFollowLink)->value;
    if (character != 0) detatch_from_character(world, main_player, main_camera, local_character3D);
    // const int edge_buffer = 8 * default_ui_scale;
    const char *game_name = "zoxel";
    // float2 window_anchor = { 0.0f, 1.0f };
    // int2 window_position = { 0 + edge_buffer, 0 - edge_buffer };
    float2 main_menu_anchor = { 0.5f, 0.5f };
    int2 main_menu_position = int2_zero;
    spawn_main_menu(world, game_name, main_menu_position, main_menu_anchor, 0);
    zox_set_only(local_game, GameState, { zoxel_game_state_main_menu })

    set_sky_color(world, menu_sky_color, menu_sky_bottom_color);
    float3 camera_position = float3_zero;
    float4 camera_rotation = quaternion_identity;
    get_camera_start_transform(&camera_position, &camera_rotation);
    zox_set_only(main_camera, Position3D, { camera_position })
    zox_set_only(main_camera, Rotation3D, { camera_rotation })
    
    disable_inputs_until_release(world, main_player);
    dispose_in_game_ui(world);
    #ifdef zox_on_play_spawn_terrain
        zox_delete(local_terrain)
    #endif
    local_character3D = 0;
    local_terrain = 0;
}

void play_game(ecs_world_t *world) {
    ecs_entity_t main_camera = main_cameras[0]; // get player camera link instead
    zoxel_log(" > game state => [main_menu] to [playing]\n");
    zox_set_only(local_game, GameState, { zoxel_game_state_playing }) // start game
    zox_delete(main_menu)   // close main menu
    zox_set_only(main_camera, VoxLink, { local_terrain })
    if (!ecs_has(world, main_camera, Streamer)) {
        zox_add_only(main_camera, Streamer)
        zox_add_only(main_camera, StreamPoint)
    }
    // \todo Fix issue with rotation, due to euler setting, make sure to set euler when spawning cameras
    #ifdef zox_on_play_spawn_terrain
        ecs_entity_t terrain = create_terrain(world);
        #ifdef zoxel_space
            zox_set_only(terrain, RealmLink, { local_realm })
        #endif
    #endif
    #ifdef zoxel_spawn_character3Ds
        spawn_many_characters3D(world);
    #endif
    #ifdef zoxel_test_single_character3Ds
        spawn_many_characters3D(world);
    #endif
    #if defined(zoxel_include_players)
        if (game_rule_attach_to_character) {
            float3 spawn_position = ecs_get(world, main_camera, Position3D)->value;
            spawn_position.x = 8;
            spawn_position.z = 8;
            float4 spawn_rotation = quaternion_identity; // ecs_get(world, main_camera, Rotation3D)->value;
            const vox_file vox = vox_files[3];
            local_character3D = spawn_chunk_character2(world, &vox, spawn_position, spawn_rotation, 0);
            // zoxel_log(" > spawned local_character3D [%lu] - spawn_position [%fx%fx%f]\n", local_character3D, spawn_position.x, spawn_position.y, spawn_position.z);
            attach_to_character(world, main_player, main_camera, local_character3D);
        } else attach_to_character(world, main_player, main_camera, 0);
    #endif
    disable_inputs_until_release(world, main_player);
    set_sky_color(world, game_sky_color, game_sky_bottom_color);
    spawn_in_game_ui(world);
}

// todo: new hotswap camera function, takes in two camera entities
/*ecs_entity_t respawn_camera(ecs_world_t *world, ecs_entity_t old_camera_entity) {
    float3 camera_position = ecs_get(world, old_camera_entity, Position3D)->value;
    float4 camera_rotation = ecs_get(world, old_camera_entity, Rotation3D)->value;
    int2 camera_screen_dimensions = ecs_get(world, old_camera_entity, ScreenDimensions)->value;
    float4x4 view_matrix = ecs_get(world, old_camera_entity, ViewMatrix)->value;
    render_camera_matrix = camera_matrix;
    ecs_entity_t e = spawn_free_camera(world, camera_position, camera_rotation, camera_screen_dimensions, (int2) { }); // spawn new free roam camera
    zox_set_only(e, ViewMatrix, { view_matrix })
    zox_delete(old_camera_entity)
    return e;
}

ecs_entity_t respawn_base_camera(ecs_world_t *world, ecs_entity_t old_camera_entity) {
    float3 camera_position = ecs_get(world, old_camera_entity, Position3D)->value;
    float4 camera_rotation = ecs_get(world, old_camera_entity, Rotation3D)->value;
    int2 camera_screen_dimensions = ecs_get(world, old_camera_entity, ScreenDimensions)->value;
    float4x4 view_matrix = ecs_get(world, old_camera_entity, ViewMatrix)->value;
    // render_camera_matrix = camera_matrix;
    ecs_entity_t e = spawn_base_camera(world, camera_position, camera_rotation, camera_screen_dimensions, (int2) { }); // spawn new free roam camera
    zox_set_only(e, ViewMatrix, { view_matrix })
    zox_delete(old_camera_entity)
    return e;
}*/