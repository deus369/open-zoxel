ecs_entity_t prefab_texture;
ecs_entity_t prefab_texture_noise;

ecs_entity_t spawn_prefab_texture_noise(ecs_world_t *world, const int2 texture_size) {
    zox_prefab()
    zox_prefab_name("prefab_texture")
    add_seed(world, e, 666);
    add_texture(world, e, texture_size, 1);
    add_noise_texture(world, e);
    prefab_texture_noise = e;
    return e;
}

ecs_entity_t spawn_prefab_texture(ecs_world_t *world, const int2 texture_size) {
    zox_prefab()
    zox_prefab_name("prefab_texture")
    add_texture_non_generate(world, e, texture_size);
    prefab_texture = e;
    return e;
}

ecs_entity_t spawn_texture_dirt(ecs_world_t *world, int seed) {
    zox_instance(prefab_texture_noise)
    zox_name("texture_dirt")
    zox_set(e, Seed, { seed })
    return e;
}
