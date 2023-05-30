
ecs_entity_t testTextureEntity;

//! Spawn a Player character.
ecs_entity_t SpawnTexture(ecs_world_t *world, ecs_entity_t prefab) {
    ecs_defer_begin(world);
    ecs_entity_t textureEntity = ecs_new_w_pair(world, EcsIsA, prefab);
    printf("Spawned TextureData [%lu]\n", (long unsigned int) textureEntity);
    ecs_defer_end(world);
    return textureEntity;
}

void TestDestroyTexture(ecs_world_t *world) {
    if (testTextureEntity && ecs_is_alive(world, testTextureEntity)) {
        printf("Deleting TextureData.\n");
        zox_delete(testTextureEntity)
    } else {
        printf("Spawning TextureData.\n");
        testTextureEntity = SpawnTexture(world, noise_texture_prefab);
    }
}
