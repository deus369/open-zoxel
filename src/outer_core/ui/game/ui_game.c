#ifndef zoxel_ui_game
#define zoxel_ui_game

// tags
//! A basic tag for a UI Element.
ECS_DECLARE(MainMenu);
ECS_DECLARE(FPSDisplay);
// components
zoxel_component(FPSDisplayTicker, double);
// zoxel_component(PixelPosition, int2);
// util

// prefabs
#include "prefabs/main_menu.c"
#include "prefabs/fps_display.c"
// systems
#include "systems/fps_display_system.c"

//! Game UI Module, main menu, level select, etc.
/**
*   \todo FPSDisplay - display current fps at top right of ui. Uses text updating.
*/
void UIGameImport(ecs_world_t *world)
{
    ECS_MODULE(world, UIGame);
    ECS_TAG_DEFINE(world, MainMenu);
    ECS_TAG_DEFINE(world, FPSDisplay);
    ECS_COMPONENT_DEFINE(world, FPSDisplayTicker);
    ECS_SYSTEM_DEFINE(world, FpsDisplaySystem, EcsOnUpdate, [out] FPSDisplayTicker, [out] ZextDirty, [out] ZextData);
    
    // ECS_COMPONENT_DEFINE(world, PixelPosition);
    // Systems
    // zoxel_system(world, ElementRaycastSystem, EcsOnUpdate, [none] ElementRaycaster);
    // prefabs
    spawn_prefab_main_menu(world);
    spawn_prefab_fps_display(world);
}
#endif