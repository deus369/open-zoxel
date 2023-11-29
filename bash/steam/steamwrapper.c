#ifndef zox_steamwrapper
#define zox_steamwrapper

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// core
unsigned char initialize_steam();
void shutdown_steam();

// local user
void authenticate_user();
const char* get_steam_user_name();

// friends
int get_friends_count();
uint64_t get_friend_id(int index);
const char* get_friend_name(uint64_t steam_id);
const char* get_friend_nickname(uint64_t steam_id);

// achievement testing
void steam_unlock_achievement(const char* achievement_name);

#ifdef __cplusplus
}
#endif

#endif
