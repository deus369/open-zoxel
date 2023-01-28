void Mesh2DUvsUpdateSystem(ecs_iter_t *it)
{
    if (!ecs_query_changed(NULL, it))
    {
        return;
    }
    const MeshDirty *meshDirtys = ecs_field(it, MeshDirty, 1);
    const MeshIndicies *meshIndicies = ecs_field(it, MeshIndicies, 2);
    const MeshVertices2D *meshVertices = ecs_field(it, MeshVertices2D, 3);
    const MeshUVs *meshUVs = ecs_field(it, MeshUVs, 4);
    const MeshGPULink *meshGPULinks = ecs_field(it, MeshGPULink, 5);
    const MaterialGPULink *materialGPULinks = ecs_field(it, MaterialGPULink, 6);
    // const UvsGPULink *uvsGPULinks = ecs_field(it, UvsGPULink, 7);
    // printf("TextureUpdateSystem [%i] \n", it->count);
    for (int i = 0; i < it->count; i++)
    {
        const MeshDirty *meshDirty = &meshDirtys[i];
        if (meshDirty->value != 1)
        {
            continue;
        }
        // printf("Mesh 2D Uploading [%i] \n", i);
        const MeshGPULink *meshGPULink = &meshGPULinks[i];
        const MaterialGPULink *materialGPULink = &materialGPULinks[i];
        const MeshIndicies *meshIndicies2 = &meshIndicies[i];
        const MeshVertices2D *meshVertices2 = &meshVertices[i];
        const MeshUVs *meshUVs2 = &meshUVs[i];
        opengl_upload_shader2D_textured(
            meshGPULink->value, materialGPULink->value,
            meshIndicies2->value, meshIndicies2->length,
            meshVertices2->value, meshVertices2->length,
            meshUVs2->value, meshUVs2->length);
    }
}
zoxel_declare_system(Mesh2DUvsUpdateSystem)