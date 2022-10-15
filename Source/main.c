// Add defines first
#include <stdbool.h>
#include <string.h>
#define SDL_IMAGES
// =-= F LECS =-=
// #define FLECS_C          /* C API convenience macros, always enabled */
#define FLECS_CUSTOM_BUILD
#define FLECS_MODULE        /* Module support */
#define FLECS_SYSTEM        /* System support */
#define FLECS_PIPELINE      /* Pipeline support */
#include "Imports/Flecs/flecs.h"
// =-= Modules =-=
// --- Core Core ---
#include "Core/Core/Core.h"
#include "Imports/Imports.h"
// --- Core ---
#include "Core/Timing/Timing.c"
#include "Core/Inputs/Inputs.h"
#include "Core/Transforms2D/Transforms2D.h"
#include "Core/Transforms/Transforms.h"
#include "Core/Rendering/Rendering.h"
#include "Core/Cameras/Cameras.h"
#include "Core/Textures/Textures.h"
#include "Core/Voxels/Voxels.h"
// --- Space ---
#include "Space/Physics2D/Physics2D.h"
// --- Gameplay ---
#include "Core/Players/Players.h"

// Settings 
bool running = true;
bool isRendering = true;
bool headless = false;
bool fullscreen = false;
bool vsync = false;
bool profiler = false;
int hasSpawnedPlayer = 0;
ecs_entity_t localPlayer;

// Forward  Declares
int ProcessArguments(int argc, char* argv[]);
void PollSDLEvents();

void ImportModules(ecs_world_t *world)
{
    // Core Modules
    ECS_IMPORT(world, Timing);
    ECS_IMPORT(world, Inputs);
    ECS_IMPORT(world, Transforms2D);
    ECS_IMPORT(world, Transforms);
    ECS_IMPORT(world, Rendering);
    ECS_IMPORT(world, Cameras);
    ECS_IMPORT(world, Textures);
    ECS_IMPORT(world, Voxels);
    // Space Modules
    ECS_IMPORT(world, Physics2D);
    // Gameplay Modules
    ECS_IMPORT(world, Players);
}

void InitializeModules(ecs_world_t *world)
{
    UpdateBeginSDL();
    // Spawn things from Modules
    InitializeInputs(world);
    InitializePlayers(world);
    InitializeCameras(world);
    // InitializeVoxels(world);
}

//! This is a mistaken function. Move along.
int main(int argc, char* argv[])
{
    int didFail = ProcessArguments(argc, argv);
    if (didFail == EXIT_FAILURE)
    {
        return EXIT_SUCCESS;
    }
    int coreCount = 24;
    if (!headless)
    {
        didFail = InitializeSDL();
        PrintSDLDebug();
        if (didFail != EXIT_FAILURE)
        {
            didFail = SpawnWindowSDL(fullscreen);
            coreCount = SDL_GetCPUCount();
            PrintOpenGL();
            if (didFail != EXIT_FAILURE)
            {
                // check open gl for failures?
                didFail = InitializeOpenGL(vsync);
            }
        }
        if (didFail == EXIT_FAILURE)
        {
            printf("Failed to Open Window. Setting to headless mode.");
            headless = true;
        }
    }
    if (headless)
    {
        isRendering = false;
    }
    BeginTime();
    InitializeECS(argc, argv, profiler, !isRendering, coreCount);
    // Import Modules Here!
    ImportModules(world);
    InitializeModules(world);
    // start game logic
    if (!headless)
    {
        SpawnMainCamera(screenDimensions);
        SpawnKeyboardEntity();
        localPlayer = SpawnPlayer(world);
    }
    //! Core Application Loop!
    printf("Entering Core Loop.\n");
    while (running)
    {
        UpdateBeginTime();
        if (!headless)
        {
            UpdateBeginSDL();
            PollSDLEvents();
        }
        if (deltaTimeSDL > 0)
        {
            if (isRendering)
            {
                UpdateBeginOpenGL(GetMainCameraViewMatrix());
            }
            if (headless && GetBobCount() < 1000000)
            {
                SpawnBobArmy(world, character2DPrefab, 250);
            }
            UpdateECS();
            if (isRendering)
            {
                UpdateEndOpenGL();
                UpdateEndSDL();
            }
        }
        if (UpdateEndTime())
        {
            const Position2D *position2D = ecs_get(world, localPlayer, Position2D);
            if (position2D)
            {
                printf("    Player position: [%fx%f]\n", position2D->value.x, position2D->value.y);
            }
            else
            {
                printf("Position2D is null.");
            }
            PrintBobSpawnSystem(world);
        }
    }
    EndECS();
    EndOpenGL();
    if (!headless)
    {
        EndSDL();
    }
    return EXIT_SUCCESS;
}

//! Temporary, quick and dirty events.
void PollSDLEvents()
{
    BobArmySpawnFixer(world);   // until bug gets fixed
    ResetKeyboard(world);
    SDL_Event event  = { 0 };
    while (SDL_PollEvent(&event))
    {
        ExtractIntoKeyboard(world, event);
        int eventType = event.type;
        if (eventType == SDL_QUIT)
        {
            running = false;
        }
        else if (eventType == SDL_KEYUP)
        {
            SDL_Keycode key = event.key.keysym.sym;
            if (key == SDLK_ESCAPE) 
            {
                running = false;
            }
            else if (key == SDLK_q) 
            {
                DestroyMainCamera();
            }
        }
        else if (eventType == SDL_WINDOWEVENT) // SDL_WINDOWEVENT_RESIZED)
        {
            if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                ResizeOpenGLViewport(event.window.data1, event.window.data2);
                ResizeCameras(event.window.data1, event.window.data2);
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
        if (strcmp(argv[i], "-z") == 0 || strcmp(argv[i], "--headless") == 0)
        {
            headless = true;
        }
    }
    return EXIT_SUCCESS;
}