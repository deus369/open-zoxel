// STD
// #include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Core/Util/SDLHelper.c"
#include "Core/Util/ECSHelper.c"

// Settings 
bool running = true;
const bool isRendering = true;
bool fullscreen = false;
bool vsync = false;
bool profiler = false;

//! Temporary, quick and dirty input.
void UpdateInput()
{
    SDL_Event event  = { 0 };
    while (SDL_PollEvent(&event))
    {
        int eventType = event.type;
        if (eventType == SDL_QUIT)
        {
            running = false;
        }
        else if (eventType == SDL_KEYUP)
        {
            SDL_Keycode key = event.key.keysym.sym;
            if (key == SDLK_q || key == SDLK_ESCAPE) 
            {
                running = false;
            }
            else if (key == SDLK_SPACE)
            {
                SpawnBobArmy();
            }
        }
        else if (eventType == SDL_WINDOWEVENT) // SDL_WINDOWEVENT_RESIZED)
        {
            /*if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                ResizeSDL(&event);
            }*/
            if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                ResizeOpenGL(event.window.data1, event.window.data2);
            }
        }
    }
}

//! Convert Args to Settings.
int ProcessArguments(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            PrintHelpMenu(argv[0]);
            return EXIT_FAILURE;
        }
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fullscreen") == 0)
        {
            fullscreen = true;
        }
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--vsync") == 0)
        {
            vsync = true;
        }
        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--profiler") == 0)
        {
            profiler = true;
        }
    }
    return EXIT_SUCCESS;
}

//! This is a mistaken function. Move along.
int main(int argc, char* argv[])
{
    int didFail = ProcessArguments(argc, argv);
    if (didFail == EXIT_FAILURE)
    {
        return EXIT_SUCCESS;
    }
    didFail = InitializeSDL();
    if (didFail == EXIT_FAILURE)
    {
        return didFail;
    }
    didFail = SpawnWindowSDL(fullscreen);
    if (didFail == EXIT_FAILURE)
    {
        return didFail;
    }
    InitializeOpenGL(vsync);
    InitializeECS(argc, argv, profiler, isRendering);
    //! Core Application Loop!
    while (running)
    {
        UpdateBeginSDL();
        UpdateInput();
        if (isRendering)
        {
            UpdateBeginOpenGL();
        }
        UpdateECS();
        if (isRendering)
        {
            UpdateEndOpenGL();
        }
        UpdateEndSDL(isRendering);
    }
    EndECS();
    EndOpenGL();
    EndSDL();
    return EXIT_SUCCESS;
}