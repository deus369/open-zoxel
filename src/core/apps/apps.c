#ifndef zoxel_apps
#define zoxel_apps

#define sdl_gl_major 3
#define sdl_gl_minor 0
#ifdef WEB_BUILD
#include <emscripten.h>
#endif
#ifdef USE_SDL_3
    #include <SDL3/SDL.h>
    #include <SDL3/SDL_opengl.h>
#else
    #ifdef ANDROID_BUILD
        #include <SDL.h>
        #include <SDL_opengl.h>
    #else
        #include <SDL2/SDL.h>
        #include <SDL2/SDL_opengl.h>
    #endif
#endif
#ifdef ANDROID_BUILD
    #ifdef SDL_IMAGES
        #include <SDL_image.h>
    #endif
#else
    #ifdef SDL_IMAGES
        #include <SDL2/SDL_image.h>
    #endif
#endif

const int window_index = 0; // 1;
unsigned char vsync = 1;
unsigned char fullscreen = 1; // set full screen default option here
unsigned char halfscreen = 0;
unsigned char is_split_screen = 0;
SDL_Window* main_window;
zoxel_declare_tag(App)
zoxel_component(SDLWindow, SDL_Window*)
zoxel_component(Renderer, SDL_Renderer*)
zoxel_component(Context, SDL_GLContext*)
#include "prefabs/app.c"
#include "util/path_util.c"
#include "util/terminal_util.c"
#include "util/webasm_util.c"
#include "util/sdl_util.c"
#include "util/cleanup_util.c"

//! apps Module.
void AppsImport(ecs_world_t *world) {
    zoxel_module(Apps)
    zoxel_define_tag(App)
    zoxel_define_component(SDLWindow)
    zoxel_define_component(Context)
    zoxel_define_component(Renderer)
    ecs_set_hooks(world, SDLWindow, { .dtor = ecs_dtor(SDLWindow) });
    ecs_set_hooks(world, Context, { .dtor = ecs_dtor(Context) });
    ecs_set_hooks(world, Renderer, { .dtor = ecs_dtor(Renderer) });
    spawn_app_prefab(world);
    set_data_path();
    // spawn app
    if (init_sdl() == EXIT_SUCCESS) {
        SDL_Window* window = spawn_sdl_window();
        SDL_GLContext* context = create_sdl_context(window);
        spawn_app(world, window, context);
        main_window = window;
        #ifndef WEB_BUILD
            #ifndef ANDROID_BUILD
                if (fullscreen) {
                    sdl_toggle_fullscreen(main_window);
                }
            #endif
        #endif
    }
}
#endif