ecs_entity_t fps_display_prefab;
ecs_entity_t fps_display;

ecs_entity_t spawn_prefab_fps_display(ecs_world_t *world) {
    ecs_defer_begin(world);
    // ecs_entity_t e = ecs_new_entity(world, "prefab_fps_display");
    // e = ecs_clone(world, e, label_prefab, true);
    ecs_entity_t e = ecs_clone(world, 0, label_background_prefab, 1); // true);
    set_unique_entity_name(world, e, "prefab_fps_display"); // bugged atm
    /*ecs_entity_t e = ecs_new_w_pair(world, EcsIsA, label_prefab);
    ecs_add_id(world, e, EcsPrefab);
    ecs_set_name(world, e, "prefab_fps_display");*/
    // has EcsPrefab?
    zox_add_tag(e, FPSDisplay);
    zox_set(e, FPSDisplayTicker, { 0 });
    prefab_set_mesh2D_vertices(world, e, square_vertices_left_aligned, 4);
    ecs_defer_end(world);
    fps_display_prefab = e;
    return e;
}

ecs_entity_t spawn_fps_display(ecs_world_t *world, ecs_entity_t parent) {
    unsigned char font_size = 32;
    byte2 padding = (byte2) { 12, 8 };
    int edge_line_thickness = canvas_edge_size * 2 + 2;
    float2 anchor = { 1.0f, 1.0f };
    int2 position = { -edge_line_thickness / 3, (- font_size / 2) - edge_line_thickness }; // x = - pixel_size.x / 2
    int layer = 1;
    const PixelSize *parent_pixel_size = ecs_get(world, parent, PixelSize);
    #ifdef zoxel_on_android
        if (anchor.y == 1) {
            position.y -= android_header_size;
        }
    #endif
    ecs_entity_t e = spawn_label_background(world, fps_display_prefab, parent, position, anchor, padding, "FPS", font_size,
        zox_zext_alignment_right, layer, (float2) { }, parent_pixel_size->value);
    return e;
}