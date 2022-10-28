// Implement better change detection on components
//      https://www.flecs.dev/flecs/#/docs/Queries?id=change-detection

//! Resets EntityDirty after one frame.
void EntityDirtyResetSystem(ecs_iter_t *it)
{
    /*if (!ecs_query_changed(NULL, it))
    {
        return;
    }*/
    // bool didChange = false;
    EntityDirty *entityDirtys = ecs_field(it, EntityDirty, 1);
    for (int i = 0; i < it->count; i++)
    {
        EntityDirty *entityDirty = &entityDirtys[i];
        if (entityDirty->value == 1)
        {
            entityDirty->value = 0;
            // didChange = true;
            // printf("EntityDirty Reseting!\n");
        }
    }
    /*if (!didChange)
    {
        ecs_query_skip(&it);
    }*/
}
ECS_SYSTEM_DECLARE(EntityDirtyResetSystem);