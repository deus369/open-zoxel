//! =-= Zoxel =-=
#define SDL_IMAGES
// game uis
// -- tests --
#define zoxel_test_character2Ds
#define zoxel_test_cubes
// #define zoxel_test_cubes_textured    // wip
// #define zoxel_test_character3Ds      // todo
#define zoxel_test_voxels
#define zoxel_test_voxels_terrain       // uvs - wip
#define zoxel_test_uis
// -- debugs --
// #define zoxel_debug_prefabs
// #define zoxel_debug_spawns
// #define zoxel_debug_sdl
// #define zoxel_debug_opengl
// #define zoxel_debug_pathing
// #define zoxel_catch_opengl_errors
// #define debug_viewport_resize
// #define zoxel_debug_element_raycasting
// #define debug_ui_positioning
// #define debug_ui_scaling
// #define zoxel_time_render_loop
#include "_includes.c"

extern bool headless;
extern bool running;
extern bool profiler;

//! Spawns our first game entities.
void spawn_game(ecs_world_t *world)
{
    spawn_main_camera(world, screenDimensions);
    spawn_ui_camera(world, screenDimensions);
    spawn_font_style(world);
    spawn_canvas(world, screenDimensions);
    #ifdef zoxel_test_uis
    spawn_test_uis(world);   // spawns test ui
    #endif
    spawn_connected_devices(world);
    spawn_player_character3D(world, get_main_camera());

    // spawn_main_menu(world, "Zoxel");
    // spawn_fps_display(world, main_canvas, 32);
    // spawn_zoxel_window(world);
}

//! The main update loop.
void update()
{
    // apps / Input events
    if (!headless)
    {
        reset_input_devices(world);
        update_sdl(world);
        #ifdef __EMSCRIPTEN__
        if (update_web_canvas(world))
        {
            return;
        }
        #endif
    }
    // ecs_log_set_level(1);    // use this to debug system pipelines
    ecs_run(world, ecs_id(ElementMeshSystem), 0, NULL);
    ecs_progress(world, 0);
    iterate_fps_time();
    // main thread, generates gpu buffer
    // ecs_run(world, ecs_id(Player2DTestMainThreadSystem), 0, NULL);
    if (!headless)
    {
        render_loop_temp();
    }
}

//! Includes special case for emscripten.
void main_loop()
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(&update, -1, 1); // old - 60, 1);
#else
    while (running)
    {
        update();
    }
#endif
}

//! Where it all begins and ends.
int main(int argc, char* argv[])
{
    int didFail = process_arguments(argc, argv);
    if (didFail == EXIT_FAILURE)
    {
        return EXIT_SUCCESS;
    }
    open_ecs(argc, argv, profiler); // begin ecs
    ECS_IMPORT(world, Zoxel);       // import game modules
    update();                       // update once? so ui does its thing properly
    spawn_game(world);              // spawn game entities
    main_loop();
    close_ecs();
    if (!headless)
    {
        dispose_opengl();
        sdl_on_end();
    }
    return 0;
}