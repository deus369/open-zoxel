#ifndef zoxel_apps
#define zoxel_apps

#define sdl_gl_major 3
#define sdl_gl_minor 0
unsigned char running = 1;
unsigned char rendering = 1;
unsigned char headless = 0;
unsigned char vsync = 1;
unsigned char fullscreen = 1; // set full screen default option here
unsigned char halfscreen = 0;
unsigned char is_split_screen = 0;
unsigned char override_opengl_es = 0;
SDL_Window* main_window;
SDL_GLContext* main_gl_context;
zox_declare_tag(App)
zox_component(SDLWindow, SDL_Window*)
zox_component(Renderer, SDL_Renderer*)
zox_component(Context, SDL_GLContext*)
#include "prefabs/app.c"
#include "util/main_loop_util.c"
#include "util/path_util.c"
#include "util/terminal_util.c"
#include "util/sdl_util.c"
#include "util/webasm_util.c"
#include "util/cleanup_util.c"
#include "util/mouse_util.c"
#include "util/platform_util.c"

zox_begin_module(Apps)
zox_define_tag(App)
zox_define_component(SDLWindow)
zox_define_component(Context)
zox_define_component(Renderer)
ecs_set_hooks(world, SDLWindow, { .dtor = ecs_dtor(SDLWindow) });
ecs_set_hooks(world, Context, { .dtor = ecs_dtor(Context) });
ecs_set_hooks(world, Renderer, { .dtor = ecs_dtor(Renderer) });
spawn_app_prefab(world);
debug_platform();
set_data_path();
if (init_sdl() == EXIT_SUCCESS) {
    create_main_window();
    print_sdl();
}
zoxel_end_module(Apps)

#endif
