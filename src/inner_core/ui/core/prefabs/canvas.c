ecs_entity_t prefab_canvas;

ecs_entity_t spawn_prefab_canvas(ecs_world_t *world) {
    zox_prefab()
    zox_prefab_name("prefab_canvas")
    zox_add_tag(e, Canvas)
    zox_prefab_set(e, PixelSize, { int2_zero })
    zox_prefab_set(e, Children, { 0, NULL })
    zox_prefab_set(e, CameraLink, { 0 })
    zox_prefab_set(e, ScreenToCanvas, { float4_zero })
    prefab_canvas = e;
    return e;
}

ecs_entity_t spawn_canvas(ecs_world_t *world, const ecs_entity_t camera, const int2 size, const float4 screen_to_canvas) {
    zox_instance(prefab_canvas)
    zox_name("canvas")
    zox_set(e, PixelSize, { size })
    zox_set(e, CameraLink, { camera })
    zox_set(e, ScreenToCanvas, { screen_to_canvas })
    return e;
}
