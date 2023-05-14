void add_chunk(ecs_world_t *world, ecs_entity_t prefab, int3 size) {
    zox_add_tag(prefab, Chunk);
    zox_add(prefab, ChunkData);
    zox_set(prefab, ChunkSize, { size });
    zox_set(prefab, ChunkDirty, { 0 });
    zox_set(prefab, ChunkPosition, { int3_zero });
    zox_set(prefab, VoxLink, { 0 });
}

void add_chunk_colors(ecs_world_t *world, ecs_entity_t e) {
    zox_add_tag(e, ColorChunk)
    zox_add(e, ColorRGBs)
    if (!headless) {
        zox_add(e, MeshColorRGBs)
    }
}

void add_generate_chunk(ecs_world_t *world, ecs_entity_t e) {
    zox_set(e, EntityDirty, { 0 });
    zox_set(e, GenerateChunk, { 1 });
}

void add_noise_chunk(ecs_world_t *world, ecs_entity_t e) {
    zox_add_tag(e, NoiseChunk);
    zox_set(e, EntityDirty, { 0 });
    zox_set(e, GenerateChunk, { 1 });
    // zox_set(e, AnimateTexture, { 0.0 });
}