//! Resets EntityDirty after one frame.
void GenerateTextureResetSystem(ecs_iter_t *it)
{
    /*if (!ecs_query_changed(NULL, it))
    {
        return;
    }*/
    GenerateTexture *generateTextures = ecs_field(it, GenerateTexture, 1);
    for (int i = 0; i < it->count; i++)
    {
        GenerateTexture *generateTexture = &generateTextures[i];
        if (generateTexture->value == 1)
        {
            generateTexture->value = 0;
            // printf("GenerateTexture Reseting!\n");
        }
    }
}
ECS_SYSTEM_DECLARE(GenerateTextureResetSystem);