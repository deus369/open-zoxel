#define max_players 16
ecs_entity_t zox_players[max_players];
unsigned char players_playing = 0;
#define zox_pip_player_movement EcsPreUpdate // EcsPreUpdate | EcsOnUpdate
// ui
const int2 crosshair_texture_size = (int2) { 128, 128 };
const int2 crosshair_pixel_size = (int2) { 64, 64 };
const color crosshair_color = (color) { 255, 255, 255, 88 };