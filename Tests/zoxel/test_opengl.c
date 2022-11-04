// Test with
//  cc -std=c99 -o test_opengl imports/Flecs/flecs.c tests/zoxel/test_opengl.c -lGL -lSDL2 -lSDL2_image -lm -D_POSIX_C_SOURCE=200112L -D_DEFAULT_SOURCE
// Run with
//  ./test_opengl
// Add defines first
#define USE_VERTEX_BUFFERS
#include <stdbool.h>
#include <string.h>
// #define SDL_IMAGES
// =-= FLECS =-=
#define FLECS_CUSTOM_BUILD
#define FLECS_MODULE
#define FLECS_SYSTEM 
#define FLECS_PIPELINE
#include "../../imports/Flecs/flecs.h"
#include "../../imports/Flecs/FlecsUtil.c"
#include "../../src/core/core/core.c"
#include "../../src/core/apps/apps.c"
#include "../../src/core/transforms2D/transforms2D.c"
#include "../../src/core/transforms/transforms.c"
#include "../../src/core/rendering/rendering.c"
#include "../../src/core/cameras/cameras.c"

int main(int argc, char* argv[])
{
    SpawnWorld(argc, argv, false, true, 4);
    ECS_IMPORT(world, apps);
    ECS_IMPORT(world, transforms2D);
    ECS_IMPORT(world, transforms);
    ECS_IMPORT(world, cameras);
    ECS_IMPORT(world, rendering);
    SpawnMainCamera(screenDimensions);
    // SpawnKeyboardEntity();
    // SpawnPlayerCharacter2D(world);
    // while true, run main loop here
    bool running = true;
    while (running)
    {
        ecs_progress(world, 0);
        UpdateBeginOpenGL(GetMainCameraViewMatrix());
        ecs_run(world, ecs_id(InstanceRender2DSystem), 0, NULL);
        UpdateEndOpenGL();
        UpdateLoopSDL();
    }
    EndECS();
    EndAppGraphics();
    EndSDL();
    return 0;
}