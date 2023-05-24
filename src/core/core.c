#ifndef zoxel_core
#define zoxel_core

unsigned char profiler = 0;
ecs_world_t *world;
#include "util/import_sdl.c"    // sdl is here
#include "util/logs.c"
#include "util/ecs/ecs.c"
#include "maths/maths.c"
#include "generic/generic.c"
#include "timing/timing.c"
#include "transforms/transforms.c"
// #include "networking/networking.c"
#include "inputs/inputs.c"
#include "apps/apps.c"
#include "cameras/cameras.c"
#include "rendering/rendering.c"
#include "sounds/sounds.c"
#include "util/core_util.c"

zox_begin_module(Core)
zox_import_module(Generic)
zox_import_module(Maths)
zox_import_module(Timing)
zox_import_module(Transforms)
// zox_import_module(Networking)
if (!headless) zox_import_module(Apps)
if (!headless) zox_import_module(Inputs)
zox_import_module(Rendering)
zox_import_module(Sounds)
zox_import_module(Cameras)
zoxel_end_module(Core)
#endif