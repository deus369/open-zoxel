void realms_game_start(ecs_world_t *world, ecs_entity_t game) {
    set_sky_color(world, game_sky_color, game_sky_bottom_color);
    if (zox_game_type == zox_game_mode_3D) {
        int3 terrain_position = int3_zero;
        create_terrain(world, terrain_position);
        zox_set(local_terrain, RealmLink, { local_realm }) // this should be done on spawn
        // link terrain to realm too
    } /*else if (zox_game_type == zox_game_mode_2D) {
        zox_log(" > todo: create terrain2D and chunk2D modules\n")
    }*/
    unlock_achievement("test_achievement"); // idk if this can be per player
}

void realms_game_end(ecs_world_t *world, ecs_entity_t game) {
    if (zox_game_type == zox_game_mode_3D) {
        zox_delete_and_set(local_terrain)
    }
    set_sky_color(world, menu_sky_color, menu_sky_bottom_color);
}

void realms_game_state(ecs_world_t *world, ecs_entity_t game, unsigned char new_game_state) {
    unsigned char previous_game_state = zox_get_value(game, GameState)
    if (previous_game_state == zox_game_start && new_game_state == zox_game_playing) realms_game_start(world, game);
    else if (new_game_state == zox_game_start) realms_game_end(world, game);
}
