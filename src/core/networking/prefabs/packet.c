/*ecs_entity_t prefab_packet;

ecs_entity_t spawn_prefab_net_player(ecs_world_t *world) {
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_prefab(world, "prefab_net_player");
    set_unique_entity_name(world, e, "prefab_net_player");
    zox_add_tag(e, NetPlayer);
    zox_add_tag(e, PacketReciever);
    zox_add_tag(e, PacketSender);
    zox_add(e, NetAddress);
    zox_add(e, TargetNetAddress);
    zox_set(e, SocketLink, { -1 });
    ecs_defer_end(world);
    #ifdef zoxel_debug_prefabs
    zoxel_log("spawn_prefab net_player [%lu].\n", (long int) (e));
    #endif
    prefab_net_player = e;
    return e;
}

ecs_entity_t spawn_net_player(ecs_world_t *world) {
    ecs_defer_begin(world);
    ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, prefab_net_player);
    set_unique_entity_name(world, e, "net_player");
    ecs_defer_end(world);
    #ifdef zoxel_debug_spawns
    zoxel_log("Spawned net_player [%lu]\n", (long int) e);
    #endif
    return e;
}*/