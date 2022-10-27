//! Helper file for Flecs!
#include <sys/sysinfo.h>
ecs_world_t *world;

//! Initialize Flecs ECS and Modules.
void BeginAppECS(int argc, char* argv[], bool profiler)
{
    world = ecs_init_w_args(argc, argv);
    // Enable Threads (if no rendering)
    // Enable Profiler
    if (profiler)
    {
        #ifdef FLECS_REST
        ECS_IMPORT(world, FlecsMonitor); 
        ecs_singleton_set(world, EcsRest, {0});
        #endif
    }
}

void SetMultiThreading()
{
    int cpuCoreCount = get_nprocs();
    int cpuCoreConfiguredCount = get_nprocs_conf();
    int sdlCoreCount = SDL_GetCPUCount();
    printf("System Found [%i processors configured] [%i processors] SDL Counts [%i]\n",
            cpuCoreConfiguredCount, cpuCoreCount, sdlCoreCount);
    if (cpuCoreCount > 1)
    {
        ecs_set_threads(world, cpuCoreCount);
    }
}

void EndAppECS()
{
    ecs_fini(world);
}