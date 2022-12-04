//! Generate random noise texture.
/**
 * Once main thread is fixed, update to gpu in this system.
 * \todo update to gpu here?
*/
void MeshUvsUpdateSystem(ecs_iter_t *it)
{
    if (!ecs_query_changed(NULL, it))
    {
        return;
    }
    const MeshDirty *entityDirtys = ecs_field(it, MeshDirty, 1);
    const MeshIndicies *meshIndicies = ecs_field(it, MeshIndicies, 2);
    const MeshVertices *meshVertices = ecs_field(it, MeshVertices, 3);
    const MeshUVs *meshUVs = ecs_field(it, MeshUVs, 4);
    const MeshGPULink *meshGPULinks = ecs_field(it, MeshGPULink, 5);
    const MaterialGPULink *materialGPULinks = ecs_field(it, MaterialGPULink, 6);
    // const UvsGPULink *uvsGPULinks = ecs_field(it, UvsGPULink, 7);
    // printf("TextureUpdateSystem [%i] \n", it->count);
    for (int i = 0; i < it->count; i++)
    {
        const MeshDirty *entityDirty = &entityDirtys[i];
        if (entityDirty->value != 1)
        {
            continue;
        }
        const MeshGPULink *meshGPULink = &meshGPULinks[i];
        const MaterialGPULink *materialGPULink = &materialGPULinks[i];
        const MeshIndicies *meshIndicies2 = &meshIndicies[i];
        const MeshVertices *meshVertices2 = &meshVertices[i];
        const MeshUVs *meshUVs2 = &meshUVs[i];
        opengl_upload_shader3D_textured(
            meshGPULink->value, materialGPULink->value,
            meshIndicies2->value, meshIndicies2->length,
            meshVertices2->value, meshVertices2->length,
            meshUVs2->value, meshUVs2->length);
        // printf("MeshUvsUpdateSystem [%i - %i] Material [%i] \n", meshGPULink->value.x, meshGPULink->value.y, materialGPULink->value);
        /*for (int j = 0; j < 8; j++)
        {
            printf("    - Vertex [%i] is [%fx%fx%f]\n", j, meshVertices2->value[j].x,  meshVertices2->value[j].y, meshVertices2->value[j].z);
        }*/
    }
}
ECS_SYSTEM_DECLARE(MeshUvsUpdateSystem);