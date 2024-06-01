
// todo: keep this in hashmap for processed Voxes - atm its gonna check 10k chunks * voxels length which is bad
// todo: support for multiple terrains using hashmap

void ChunkOctreeBuildSystem(ecs_iter_t *it) {
#ifdef zox_disable_chunk_building
    return;
#endif
    zox_change_check()
#ifdef zox_octree_chunk_build_limits
    int chunks_built = 0;
#endif
    zox_iter_world()
    zox_field_in(VoxLink, voxLinks, 5)
    int voxels_length = 0;
    ecs_entity_t terrain;
    for (int i = 0; i < it->count; i++) {
        zox_field_i(VoxLink, voxLinks, voxLink)
        if (!voxLink->value) continue;
        const ecs_entity_t realm = zox_get_value(voxLink->value, RealmLink)
        const VoxelLinks *voxelLinks = zox_get(realm, VoxelLinks)
        voxels_length = voxelLinks->length;
        terrain = voxLink->value;
        break;
    }
    if (voxels_length == 0 || terrain == 0) return; // if failed to find terrain parents
    const ecs_entity_t tilemap = zox_get_value(terrain, TilemapLink)
    const TilemapUVs *tilemapUVs = zox_get(tilemap, TilemapUVs)
    if (tilemapUVs->value == NULL || tilemapUVs->length == 0) return; // if tilemap generating still
    ChunkTexturedBuildData build_data;
    unsigned char solidity[voxels_length];
    int uvs[voxels_length * 6 * sizeof(int)];
    build_data.solidity = solidity;
    build_data.uvs = uvs;
    // memset(block_voxes, 0, block_voxes_count * sizeof(ecs_entity_t));
    // memset(block_vox_offsets, 0, block_voxes_count);
    const ecs_entity_t realm = zox_get_value(terrain, RealmLink)
    const VoxelLinks *voxelLinks = zox_get(realm, VoxelLinks)
    // calculate tileuv indexes - voxel and face to index  in tilemapUVs
    int uvs_index = 0;
    for (int j = 0; j < voxelLinks->length; j++) {
        const ecs_entity_t block = voxelLinks->value[j];
        // solidity
        if (!zox_has(block, BlockModel)) build_data.solidity[j] = 1;
        else build_data.solidity[j] = zox_gett_value(block, BlockModel) == zox_block_solid;
        // uvs
        const unsigned char block_textures_length = zox_gett(block, Textures)->length;
        int voxel_uv_indexes_index = j * 6;
        if (block_textures_length == 1) {
            // per voxel, 24 uvs
            for (unsigned char k = 0; k < 6; k++) build_data.uvs[voxel_uv_indexes_index + k] = uvs_index;
            uvs_index += 4;
        } else {
            // for 6 sides textured voxes
            for (unsigned char k = 0; k < 6; k++) {
                build_data.uvs[voxel_uv_indexes_index + k] = uvs_index;
                uvs_index += 4;
            }
        }
    }
    // main loop to build
    zox_field_in(ChunkOctree, chunkOctrees, 2)
    zox_field_in(RenderLod, renderLods, 3)
    zox_field_in(ChunkNeighbors, chunkNeighbors, 4)
    zox_field_in(VoxScale, voxScales, 11)
    zox_field_out(ChunkDirty, chunkDirtys, 1)
    zox_field_out(MeshIndicies, meshIndiciess, 6)
    zox_field_out(MeshVertices, meshVerticess, 7)
    zox_field_out(MeshUVs, meshUVss, 8)
    zox_field_out(MeshColorRGBs, meshColorRGBss, 9)
    zox_field_out(MeshDirty, meshDirtys, 10)
    for (int i = 0; i < it->count; i++) {
        zox_field_o(ChunkDirty, chunkDirtys, chunkDirty)
        if (!chunkDirty->value) continue;
        zox_field_i(RenderLod, renderLods, renderLod)
        zox_field_i(VoxScale, voxScales, voxScale)
        const unsigned char lod = get_terrain_lod_from_camera_distance(renderLod->value);
        zox_field_o(MeshIndicies, meshIndiciess, meshIndicies)
        zox_field_o(MeshVertices, meshVerticess, meshVertices)
        zox_field_o(MeshColorRGBs, meshColorRGBss, meshColorRGBs)
        zox_field_o(MeshUVs, meshUVss, meshUVs)
        zox_field_o(MeshDirty, meshDirtys, meshDirty)
        if (lod == 255) { // hides mesh
            clear_mesh_uvs(meshIndicies, meshVertices, meshColorRGBs, meshUVs);
            chunkDirty->value = 0;
            meshDirty->value = 1;
            continue;
        }
        // zox_field_i(VoxLink, voxLinks, voxLink)
        zox_field_i(ChunkOctree, chunkOctrees, chunkOctree)
        zox_field_i(ChunkNeighbors, chunkNeighbors, chunkNeighbors2)
        const ChunkOctree *neighbors[6];
        unsigned char neighbor_lods[6];
        set_neightbor_chunk_data(left)
        set_neightbor_chunk_data(right)
        set_neightbor_chunk_data(down)
        set_neightbor_chunk_data(up)
        set_neightbor_chunk_data(back)
        set_neightbor_chunk_data(front)
        build_chunk_octree_mesh_uvs(chunkOctree, tilemapUVs, meshIndicies, meshVertices, meshUVs, meshColorRGBs, renderLod->value, lod, neighbors, neighbor_lods, voxScale->value, build_data.solidity, build_data.uvs);
        chunkDirty->value = 0;
        meshDirty->value = 1;
#ifdef zox_octree_chunk_build_limits
        chunks_built++;
        if (chunks_built >= max_chunks_build_per_frame) break;
#endif
    }
} zox_declare_system(ChunkOctreeBuildSystem)
