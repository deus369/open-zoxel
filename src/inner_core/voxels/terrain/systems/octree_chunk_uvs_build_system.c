void count_mesh(const ChunkOctree *chunkOctree,
    MeshIndicies *meshIndicies, MeshVertices *meshVertices, MeshUVs *meshUVs,
    int2 *mesh_count)
{
    int3 local_position;
    for (local_position.x = 0; local_position.x < octree_node_size; local_position.x++)
    {
        for (local_position.y = 0; local_position.y < octree_node_size; local_position.y++)
        {
            for (local_position.z = 0; local_position.z < octree_node_size; local_position.z++)
            {
                int array_index = int3_array_index(local_position, octree_node_size3);
                if (chunkOctree->nodes[array_index].value == 0)
                {
                    continue;
                }
                if (chunkOctree->nodes[array_index].nodes != NULL)
                {
                    count_mesh(&chunkOctree->nodes[array_index], meshIndicies, meshVertices, meshUVs, mesh_count);
                }
                else
                {
                    // add faces - based on neighbor voxels
                    zoxel_add_voxel_face_counts()
                    zoxel_add_voxel_face_counts()
                    zoxel_add_voxel_face_counts()
                    zoxel_add_voxel_face_counts()
                    zoxel_add_voxel_face_counts()
                    zoxel_add_voxel_face_counts()
                }
            }
        }
    }
}

void build_octree_chunk(const ChunkOctree *chunkOctree,
    MeshIndicies *meshIndicies, MeshVertices *meshVertices, MeshUVs *meshUVs,
    int2 *start, float3 parent_vertex_position_offset, float scale)
{
    float voxel_scale = scale * (overall_voxel_scale / ((float) octree_node_size));
    scale *= 0.5f;
    float3 center_mesh_offset = (float3) { - overall_voxel_scale / 2.0f,
        - overall_voxel_scale / 2.0f, - overall_voxel_scale / 2.0f };
    int3 local_position;
    for (local_position.x = 0; local_position.x < octree_node_size; local_position.x++)
    {
        for (local_position.y = 0; local_position.y < octree_node_size; local_position.y++)
        {
            for (local_position.z = 0; local_position.z < octree_node_size; local_position.z++)
            {
                int array_index = int3_array_index(local_position, octree_node_size3);
                ChunkOctree node = chunkOctree->nodes[array_index];
                if (node.value == 0)
                {
                    continue;
                }
                float3 vertex_position_offset = float3_add(parent_vertex_position_offset,
                    float3_multiply_float(float3_from_int3(local_position), voxel_scale));
                if (chunkOctree->nodes[array_index].nodes != NULL)
                {
                    build_octree_chunk(&chunkOctree->nodes[array_index],
                        meshIndicies, meshVertices, meshUVs, start, vertex_position_offset, scale);
                }
                else
                {
                    zoxel_add_voxel_face_uvs(left, 0)
                    zoxel_add_voxel_face_uvs(right, 1)
                    zoxel_add_voxel_face_uvs(down, 1)
                    zoxel_add_voxel_face_uvs(up, 0)
                    zoxel_add_voxel_face_uvs(back, 0)
                    zoxel_add_voxel_face_uvs(front, 1)
                }
            }
        }
    }
}

#define float3_zero (float3) { 0, 0, 0 }

void build_octree_chunk_mesh_uvs(const ChunkOctree *chunkOctree, const ChunkSize *chunkSize,
    MeshIndicies *meshIndicies, MeshVertices *meshVertices, MeshUVs *meshUVs,
    const ChunkOctree *chunk_left, const ChunkOctree *chunk_right,
    const ChunkOctree *chunk_back, const ChunkOctree *chunk_front)
{
    int2 mesh_count = { 0, 0 };
    int2 *start = &((int2) { 0, 0 });
    //int indicies_count = 0;
    //int verticies_count = 0;
    //int uvs_count = 0;
    count_mesh(chunkOctree, meshIndicies, meshVertices, meshUVs, &mesh_count);
    re_initialize_memory_component(meshIndicies, int, mesh_count.x);
    re_initialize_memory_component(meshVertices, float3, mesh_count.y);
    re_initialize_memory_component(meshUVs, float2, mesh_count.y);
    // printf("indicies_count [%i]\n", mesh_count.x);
    build_octree_chunk(chunkOctree, meshIndicies, meshVertices, meshUVs, start, float3_zero, 1.0f);
    // int3 local_position;
    //! Precount our index and vertex array lengths.
    /*for (local_position.x = 0; local_position.x < octree_node_size; local_position.x++)
    {
        for (local_position.y = 0; local_position.y < octree_node_size; local_position.y++)
        {
            for (local_position.z = 0; local_position.z < octree_node_size; local_position.z++)
            {
                int array_index = int3_array_index(local_position, octree_node_size3);
                if (chunkOctree->nodes[array_index].value == 0)
                {
                    continue;
                }
                if (chunkOctree->nodes[array_index].nodes == NULL)
                {
                    continue;
                }
                // add faces - based on neighbor voxels
                zoxel_add_voxel_face_counts()
                zoxel_add_voxel_face_counts()
                zoxel_add_voxel_face_counts()
                zoxel_add_voxel_face_counts()
                zoxel_add_voxel_face_counts()
                zoxel_add_voxel_face_counts()
            }
        }
    }*/
    //! Create our index and vertex arrays
    /*for (local_position.x = 0; local_position.x < octree_node_size; local_position.x++)
    {
        for (local_position.y = 0; local_position.y < octree_node_size; local_position.y++)
        {
            for (local_position.z = 0; local_position.z < octree_node_size; local_position.z++)
            {
                int array_index = int3_array_index(local_position, octree_node_size3);
                ChunkOctree node = chunkOctree->nodes[array_index];
                if (node.value == 0)
                {
                    continue;
                }
                float3 vertex_position_offset = float3_multiply_float(float3_from_int3(local_position), voxel_scale);
                if (chunkOctree->nodes[array_index].nodes != NULL)
                {
                    int3 local_position2;
                    for (local_position2.x = 0; local_position2.x < octree_node_size; local_position2.x++)
                    {
                        for (local_position2.y = 0; local_position2.y < octree_node_size; local_position2.y++)
                        {
                            for (local_position2.z = 0; local_position2.z < octree_node_size; local_position2.z++)
                            {

                            }
                        }
                    }
                    continue;
                }
                zoxel_add_voxel_face_uvs(left, 0)
                zoxel_add_voxel_face_uvs(right, 1)
                zoxel_add_voxel_face_uvs(down, 1)
                zoxel_add_voxel_face_uvs(up, 0)
                zoxel_add_voxel_face_uvs(back, 0)
                zoxel_add_voxel_face_uvs(front, 1)
            }
        }
    }*/
}

//! Builds a mesh data from the chunk!
void OctreeChunkUVsBuildSystem(ecs_iter_t *it)
{
    ecs_query_t *changeQuery = it->ctx;
    if (!changeQuery || !ecs_query_changed(changeQuery, NULL))
    {
        return;
    }
    // printf("[ChunkBuildSystem] GenerateChunk was changed.\n");
    const ChunkDirty *entityDirtys = ecs_field(it, ChunkDirty, 1);
    const ChunkOctree *chunkOctrees = ecs_field(it, ChunkOctree, 2);
    const ChunkSize *chunkSizes = ecs_field(it, ChunkSize, 3);
    const ChunkNeighbors *chunkNeighbors = ecs_field(it, ChunkNeighbors, 4);
    MeshIndicies *meshIndicies = ecs_field(it, MeshIndicies, 5);
    MeshVertices *meshVertices = ecs_field(it, MeshVertices, 6);
    MeshUVs *meshUVs = ecs_field(it, MeshUVs, 7);
    MeshDirty *meshDirtys = ecs_field(it, MeshDirty, 8);
    for (int i = 0; i < it->count; i++)
    {
        const ChunkDirty *chunkDirty = &entityDirtys[i];
        if (chunkDirty->value == 0)
        {
            continue;
        }
        MeshDirty *meshDirty = &meshDirtys[i];
        if (meshDirty->value != 0)
        {
            continue;
        }
        meshDirty->value = 1;
        const ChunkOctree *chunkOctree = &chunkOctrees[i];
        const ChunkSize *chunkSize = &chunkSizes[i];
        const ChunkNeighbors *chunkNeighbors2 = &chunkNeighbors[i];
        MeshIndicies *meshIndicies2 = &meshIndicies[i];
        MeshVertices *meshVertices2 = &meshVertices[i];
        MeshUVs *meshUVs2 = &meshUVs[i];
        const ChunkOctree *chunk_left = chunkNeighbors2->value[0] == 0 ?
            NULL : ecs_get(it->world, chunkNeighbors2->value[0], ChunkOctree);
        const ChunkOctree *chunk_right = chunkNeighbors2->value[1] == 0 ?
            NULL : ecs_get(it->world, chunkNeighbors2->value[1], ChunkOctree);
        const ChunkOctree *chunk_back = chunkNeighbors2->value[2] == 0 ?
            NULL : ecs_get(it->world, chunkNeighbors2->value[2], ChunkOctree);
        const ChunkOctree *chunk_front = chunkNeighbors2->value[3] == 0 ?
            NULL : ecs_get(it->world, chunkNeighbors2->value[3], ChunkOctree);
        build_octree_chunk_mesh_uvs(chunkOctree, chunkSize, meshIndicies2, meshVertices2, meshUVs2,
            chunk_left, chunk_right, chunk_back, chunk_front);
        // printf("Building ChunkData UVs Mesh [%lu]\n", (long int) it->entities[i]);
    }
}
zoxel_declare_system(OctreeChunkUVsBuildSystem)

                /*#ifndef disable_voxel_left
                zoxel_check_faces_with_uvs(left)
                #endif
                #ifndef disable_voxel_right
                zoxel_check_faces_with_uvs(right)
                #endif
                #ifndef disable_voxel_down
                zoxel_check_faces_no_chunk(down)
                #endif
                #ifndef disable_voxel_up
                zoxel_check_faces_no_chunk(up)
                #endif
                #ifndef disable_voxel_back
                zoxel_check_faces_with_uvs(back)
                #endif
                #ifndef disable_voxel_front
                zoxel_check_faces_with_uvs(front)
                #endif*/

                /*#ifndef disable_voxel_left
                zoxel_add_faces_with_uvs(left, 0)
                #endif
                #ifndef disable_voxel_right
                zoxel_add_faces_with_uvs(right, 1)
                #endif
                #ifndef disable_voxel_down
                zoxel_add_faces_no_chunk(down, 1)
                #endif
                #ifndef disable_voxel_up
                zoxel_add_faces_no_chunk(up, 0)
                #endif
                #ifndef disable_voxel_back
                zoxel_add_faces_with_uvs(back, 0)
                #endif
                #ifndef disable_voxel_front
                zoxel_add_faces_with_uvs(front, 1)
                #endif*/
                    /*int3 local_position2;
                    for (local_position2.x = 0; local_position2.x < octree_node_size; local_position2.x++)
                    {
                        for (local_position2.y = 0; local_position2.y < octree_node_size; local_position2.y++)
                        {
                            for (local_position2.z = 0; local_position2.z < octree_node_size; local_position2.z++)
                            {

                            }
                        }
                    }*/