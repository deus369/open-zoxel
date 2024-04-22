#ifndef zoxel_games
#define zoxel_games

unsigned char zox_game_type;
zox_declare_tag(Game)
zox_component_byte(GameState);
#include "data/game_modes.c"
#include "data/game_states.c"
#include "data/zox_game_event.c"
#include "prefabs/game.c"
#include "util/game_events.c"
// #include "systems/play_game_system.c"

void initialize_games(ecs_world_t *world) {
    initialize_events_games();
}

void dispose_games(ecs_world_t *world) {
    dispose_events_games();
}

void spawn_prefabs_games(ecs_world_t *world) {
    spawn_prefab_game(world);
}

zox_begin_module(Games)
zox_define_tag(Game)
zox_define_component(GameState)
// zox_system(PlayGameSystem, EcsPostUpdate, [in] GenericEvent)
zoxel_end_module(Games)

#endif
