// grid like ui
ecs_entity_t spawn_prefab_taskbar(ecs_world_t *world, const ecs_entity_t prefab) {
    zox_prefab_child(prefab)
    zox_prefab_name("taskbar")
    zox_add_tag(e, Taskbar)
    zox_prefab_add(e, Children)
    return e;
}

// this is created in our users module
extern void set_taskbar_icon_active(ecs_world_t *world, const ecs_entity_t canvas, const ecs_entity_t frame, const int i);
extern void on_toggle_taskbar_icon(ecs_world_t *world, const ecs_entity_t clicker, const ecs_entity_t clicked);

ecs_entity_t spawn_taskbar(ecs_world_t *world, const ecs_entity_t prefab, const ecs_entity_t canvas, const ecs_entity_t parent, const unsigned char layer) {
    int2 position = (int2) { 0, -24 };
    float2 anchor = (float2) {0.5f, 1 };
    int size = 48;
    const int actions_count = 10;
    const int padding_x = 4;
    const int padding_y = 4;
    const int margins = size / 4;
    // const color fill_color = (color) { 22, 77, 77, 122 };
    // const color outline_color = (color) { 11, 255, 11, 122 };
    const int2 canvas_size = zox_get_value(canvas, PixelSize)
    const int2 actionbar_size = (int2) { padding_x + (size + padding_x) * actions_count + margins * 2, size + padding_y * 2 };
    ElementSpawn spawn_actionbar = {
        .canvas = {
            .e = canvas,
            .size = canvas_size
        },
        .parent = {
            .e = parent,
            .position = int2_half(canvas_size),
            .size = canvas_size
        },
        .element = {
            .prefab = prefab,
            .layer = layer,
            .anchor = anchor,
            .position = position,
            .size = actionbar_size
        },
        .texture = {
            .fill_color = default_fill_color_window,
            .outline_color = default_outline_color_window,
        }
    };
    const ecs_entity_t e = spawn_element(world, &spawn_actionbar);
    zox_prefab_add(e, Children)
    zox_get_muter(e, Children, children)
    initialize_memory_component(Children, children, ecs_entity_t, actions_count)
    const int2 action_frame_size = (int2) { size, size };
    const int2 icon_size = (int2) { size - 6, size - 6 };
    ElementSpawn spawn_frame_data = {
        .canvas = spawn_actionbar.canvas,
        .parent = {
            .e = e,
            .position = position,
            .size = actionbar_size
        },
        .element = {
            .prefab = prefab_icon_frame,
            .layer = layer + 1,
            .anchor = float2_half,
            .size = action_frame_size,
        },
        .texture = {
            .fill_color = default_fill_color_frame,
            .outline_color = default_outline_color_frame,
        }
    };
    ElementSpawn spawn_icon_data = {
        .canvas = spawn_actionbar.canvas,
        .parent = {
            .size = spawn_frame_data.element.size
        },
        .element = {
            .prefab = prefab_icon,
            .layer = layer + 2,
            .anchor = float2_half,
            .size = icon_size
        },
        .texture = {
            .fill_color = default_fill_color_icon,
            .outline_color = default_outline_color_icon,
        }
    };
    for (int i = 0; i < actions_count; i++) {
        const int position_x = (int) ((i - (actions_count / 2) + 0.5f) * (size + padding_x));
        spawn_frame_data.element.position = (int2) { position_x, 0 };
        const ecs_entity_t frame = spawn_element(world, &spawn_frame_data);
        children->value[i] = frame;

        // Active State
        if (i == 0) zox_set(frame, ActiveState, { 1 })
        set_taskbar_icon_active(world, canvas, frame, i);

        if (i <= 9) {
            zox_get_muter(frame, Children, frame_children)
            initialize_memory_component(Children, frame_children, ecs_entity_t, 1)
            spawn_icon_data.parent.e = frame;
            spawn_icon_data.parent.position = spawn_frame_data.element.position;

            // texture
            char *icon_texture_name = "";
            if (i == 0) icon_texture_name = "LogoutUI";
            else if (i == 1) icon_texture_name = "StatusUI";
            else if (i == 2) icon_texture_name = "stat_points";
            else if (i == 3) icon_texture_name = "InventoryUI";
            else if (i == 4) icon_texture_name = "EquipUI - Copy";
            else if (i == 5) icon_texture_name = "EquipUI";
            else if (i == 6) icon_texture_name = "SpellsUI";
            else if (i == 7) icon_texture_name = "CraftUI";
            else if (i == 8) icon_texture_name = "QuestsUI";
            else if (i == 9) icon_texture_name = "CombatUI";
            else if (i == 10) icon_texture_name = "MapUI";

            const ecs_entity_t test_icon_texture = string_hashmap_get(files_hashmap_textures, new_string_data(icon_texture_name));

            const ecs_entity_t icon2 = spawn_element(world, &spawn_icon_data);
            frame_children->value[0] = icon2;
            zox_set(icon2, ClickEvent, { &on_toggle_taskbar_icon })
            zox_set(icon2, IconIndex, { i })

            zox_set(icon2, GenerateTexture, { zox_generate_texture_none })
            clone_texture_data(world, icon2, test_icon_texture);
            zox_set(icon2, TextureDirty, { 1 })
        }
    }
}
