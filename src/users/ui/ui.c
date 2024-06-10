#ifndef zox_mod_users_ui
#define zox_mod_users_ui

zox_declare_tag(UserIcon)
zox_declare_tag(UserIconFrame)
zox_declare_tag(UserIconWindow)
zox_declare_tag(MenuStatus)     // a general ui with a summary of all user data, Name, Race, Level, etc
#include "data/spawn_window_users.c"
#include "prefabs/prefabs.c"
#include "systems/user_icon_frame_system.c"
#include "util/menu_macro.c"

zox_begin_module(UsersUI)
zox_define_tag(UserIcon)
zox_define_tag(UserIconFrame)
zox_define_tag(UserIconWindow)
zox_define_tag(MenuStatus)
zox_system(UserIconFrameSystem, EcsOnUpdate, [in] SelectState, [in] UserDataLink, [in] CanvasLink, [in] TooltipEvent, [none] UserIcon)
spawn_prefabs_users_ui(world);
zoxel_end_module(Users)

#endif