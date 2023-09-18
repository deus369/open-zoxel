// todo: canvas lines doesn't update in this

extern int2 get_line_element_mid_point(ecs_world_t *world, ecs_entity_t e);
extern void set_line_element_real_position2D(ecs_world_t *world, ecs_entity_t e, float2 real_position2D, float2 canvas_size_f, float aspect_ratio, int2 pixel_position);

void set_element_position(ecs_world_t *world, ecs_entity_t e, float2 parent_position, int2 parent_pixel_size, float2 canvas_size_f, float aspect_ratio) {
    if (e == 0 || !ecs_is_alive(world, e)) return; // || !ecs_is_valid(world, e)
    int2 pixel_position = int2_zero;
    if (ecs_has(world, e, PixelPosition)) {
        pixel_position = ecs_get(world, e, PixelPosition)->value;
    } else {
        pixel_position = get_line_element_mid_point(world, e);
    }
    float2 anchor = float2_zero;    // should i pass this in from parent?
    if (ecs_has(world, e, Anchor)) {
        anchor = ecs_get(world, e, Anchor)->value;
    }
    float2 real_position2D = get_ui_real_position2D_parent(pixel_position, anchor, parent_position, parent_pixel_size, canvas_size_f, aspect_ratio);
    if (ecs_has(world, e, Position2D)) {
        Position2D *position2D = ecs_get_mut(world, e, Position2D);
        position2D->value = real_position2D;
        ecs_modified(world, e, Position2D);
    }
    if (ecs_has(world, e, CanvasPixelPosition)) {
        CanvasPixelPosition *canvasPixelPosition = ecs_get_mut(world, e, CanvasPixelPosition);
        canvasPixelPosition->value = (int2) { ceil((real_position2D.x / aspect_ratio + 0.5f) * canvas_size_f.x), ((real_position2D.y + 0.5f) * canvas_size_f.y) };
        ecs_modified(world, e, CanvasPixelPosition);
    }
    set_line_element_real_position2D(world, e, real_position2D, canvas_size_f, aspect_ratio, pixel_position);
    if (ecs_has(world, e, Children)) {
        int2 pixel_size = ecs_get(world, e, PixelSize)->value;
        const Children *children = ecs_get(world, e, Children);
        for (int i = 0; i < children->length; i++) set_element_position(world, children->value[i], real_position2D, pixel_size, canvas_size_f, aspect_ratio);
    }
}

// moves ui around after repositioning the pixel position
void ElementPositionSystem(ecs_iter_t *it) {
    ecs_query_t *changeQuery = it->ctx;
    ecs_iter_t change_iter = ecs_query_iter(it->world, changeQuery);
    while (ecs_query_next(&change_iter)) {
        if (change_iter.table != it->table) ecs_query_skip(&change_iter);
    }
    ecs_world_t *world = it->world;
    const PixelPosition *pixelPositions = ecs_field(it, PixelPosition, 2);
    const ParentLink *parentLinks = ecs_field(it, ParentLink, 3);
    const Anchor *anchors = ecs_field(it, Anchor, 4);
    const CanvasLink *canvasLinks = ecs_field(it, CanvasLink, 5);
    Position2D *position2Ds = ecs_field(it, Position2D, 6);
    CanvasPixelPosition *canvasPixelPositions = ecs_field(it, CanvasPixelPosition, 7);
    for (int i = 0; i < it->count; i++) {
        ecs_entity_t e = it->entities[i];
        const PixelPosition *pixelPosition = &pixelPositions[i];
        const ParentLink *parentLink = &parentLinks[i];
        const Anchor *anchor = &anchors[i];
        const CanvasLink *canvasLink = &canvasLinks[i];
        if (!ecs_is_valid(world, canvasLink->value) || parentLink->value != canvasLink->value) continue;
        int2 canvas_size = ecs_get(world, canvasLink->value, PixelSize)->value;
        float2 canvas_size_f = { (float) canvas_size.x, (float) canvas_size.y };
        float aspect_ratio = canvas_size_f.x / canvas_size_f.y;
        Position2D *position2D = &position2Ds[i];
        CanvasPixelPosition *canvasPixelPosition = &canvasPixelPositions[i];
        position2D->value = get_ui_real_position2D_canvas(pixelPosition->value, anchor->value, canvas_size_f, aspect_ratio);
        canvasPixelPosition->value = (int2) { ceil((position2D->value.x / aspect_ratio + 0.5f) * canvas_size_f.x), ((position2D->value.y + 0.5f) * canvas_size_f.y) };
        #ifdef debug_element_position_change_query
            zoxel_log("    - PixelPosition Updated [%lu]\n", (long int) e);
        #endif
        if (ecs_has(world, e, Children)) {
            int2 pixel_size = ecs_get(world, e, PixelSize)->value;
            const Children *children = ecs_get(world, e, Children);
            for (int i = 0; i < children->length; i++) {
                set_element_position(world, children->value[i], position2D->value, pixel_size, canvas_size_f, aspect_ratio);
            }
        }
    }
} zox_declare_system(ElementPositionSystem)

//   \done Make this instantaneous for children uis as well
//   \todo This needs to also account for child uis
//   \todo Change queries still not working, make a better test function with more components

    /*if (!ecs_query_changed(changeQuery, NULL))
    {
        return;
    }*/
    //! \todo Update this after getting a working example of change filters
    /*#ifdef debug_element_position_change_query
        unsigned char did_change = ecs_query_changed(changeQuery, NULL);
    #endif*/