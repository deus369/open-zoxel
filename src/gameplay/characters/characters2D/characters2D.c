#ifndef Zoxel_Characters2D
#define Zoxel_Characters2D
//! players Module

const int spawnCharacter2DsCount = 64;
const int2 character2DTextureSize = { 16, 16 };
// Tags
ECS_DECLARE(Character2D);
// components
zoxel_component(Character2DLink, ecs_entity_t);
// prefabs
#include "prefabs/character2D.c"
// util
#include "util/character2D.c"
// systems
#include "systems/BobSpawnSystem.c"

//! Testing, spawn more!
void SpawnManyCharacters()
{
    ecs_entity_t first_character;
    for (int i = 0; i < spawnCharacter2DsCount; i++)
    {
        ecs_entity_t e = spawn_character2D(world, character2DPrefab, (float2) { -0.5f + (rand() % 100) * 0.2f, -0.5f + (rand() % 100) * 0.2f });
        if (i == 0)
        {
            first_character = e;
            printf("Inside Loop - MaterialGPULink [%lu] : %i\n", (long int) first_character, ecs_get(world, first_character, MaterialGPULink)->value);
        }
    }
    printf("Outside Loop - MaterialGPULink [%lu] : %i\n", (long int) first_character, ecs_get(world, first_character, MaterialGPULink)->value);
}

//! A module for 2 dimensional characters.
void Characters2DImport(ecs_world_t *world)
{
    ECS_MODULE(world, Characters2D);
    ECS_TAG_DEFINE(world, Character2D);
    ECS_COMPONENT_DEFINE(world, Character2DLink);
    spawn_character2D_prefab(world, character2DTextureSize);
    SpawnManyCharacters();
}
#endif