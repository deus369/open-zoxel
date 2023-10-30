extern ecs_entity_t local_realm;
extern ecs_entity_t local_terrain;
extern void add_realm_entity_to_labels(ecs_world_t *world, ecs_entity_t e, text_group_dynamic_array_d* labels, ecs_entity_t_array_d* entities, int tree_level);
const int hierarchy_max_line_characters = 64;
ecs_entity_t editor_selected;

void add_entity_to_labels(ecs_world_t *world, ecs_entity_t e, text_group_dynamic_array_d* labels, ecs_entity_t_array_d* entities, int tree_level) {
    if (!e) return;
    unsigned char *text = malloc(hierarchy_max_line_characters);
    if (!zox_has(e, ZoxName)) {
        snprintf(text, hierarchy_max_line_characters, "_%lu [%i]", e, tree_level);
    } else {
        const ZoxName *zoxName = zox_get(e, ZoxName)
        char *text2 = convert_zext_to_text(zoxName->value, zoxName->length);
        snprintf(text, hierarchy_max_line_characters, "%s", text2);
        free(text2);
        for (int i = 0; i < tree_level; i++) {
            char *temp = strdup(text);
            snprintf(text, hierarchy_max_line_characters, "-%s", temp);
            free(temp);
        }
    }
    add_to_text_group_dynamic_array_d(labels, (text_group_dynamic) { text = text });
    add_to_ecs_entity_t_array_d(entities, e);
}

int get_max_characters_d(const char *header_label, text_group_dynamic_array_d* labels) {
    int max_characters = 0; // get max text length out of all of the words
    for (int i = 0; i < labels->size; i++) {
        int txt_size = strlen(labels->data[i].text);
        if (txt_size > max_characters) max_characters = txt_size;
    }
    int header_txt_size = strlen(header_label);
    if (header_txt_size > max_characters) max_characters = header_txt_size;
    return max_characters;
}

void add_entity_children_to_labels(ecs_world_t *world, ecs_entity_t e, text_group_dynamic_array_d* labels, ecs_entity_t_array_d* entities, int tree_level) {
    if (!e) return;
    add_entity_to_labels(world, e, labels, entities, tree_level);
    if (zox_has(e, Children)) {
        tree_level++;
        const Children *children = zox_get(e, Children)
        for (int i = 0; i < children->length; i++) add_entity_children_to_labels(world, children->value[i], labels, entities, tree_level);
    }
}

void print_entity(ecs_world_t *world, ecs_entity_t e) {
    const ecs_type_t *type = ecs_get_type(world, e);
    const ecs_id_t *type_ids = type->array;
    int32_t i, count = type->count;

    char *type_str = ecs_type_str(world, type);
    zox_log("       > ecs_types: %s\n", type_str)
    free(type_str);

    for (i = 0; i < count; i ++) {
        ecs_id_t id = type_ids[i];
        zox_logg("       > ")
        if (ECS_HAS_ID_FLAG(id, PAIR)) {
            ecs_entity_t rel = ecs_pair_first(world, id);
            ecs_entity_t tgt = ecs_pair_second(world, id);
            zox_log(" pair %s [%lu]", ecs_get_name(world, rel), tgt)
        } else {
            ecs_entity_t comp = id & ECS_COMPONENT_MASK;
            zox_log(" %lu", comp)
            zox_log(" %s", ecs_get_name(world, comp))
            // how to print void*, get*/ base type of ->value of component
            // zox_log(" [%i]", ecs_get_id(world, target, id)->value)
            if (is_component_type_byte(id)) {
                const EntityDirty *byte_component = ((const EntityDirty*) ecs_get_id(world, e, id));
                if (byte_component) zox_log(" b [%i]", byte_component->value)
            } else if (is_component_type_long_int(id)) {
                const Seed *long_int_component = ((const Seed*) ecs_get_id(world, e, id));
                if (long_int_component) zox_log(" lu [%lu]", long_int_component->value)
            } else if (is_component_type_int2(id)) {
                const DraggingDelta *int2_component = ((const DraggingDelta*) ecs_get_id(world, e, id));
                if (int2_component) zox_log(" i2 [%ix%i]", int2_component->value.x, int2_component->value.y)
            } else if (is_component_type_int(id)) {
                const ID *int_component = ((const ID*) ecs_get_id(world, e, id));
                if (int_component) zox_log(" i [%i]", int_component->value)
            } else {
                const ZoxName *zoxName = ((const ZoxName*) ecs_get_id(world, e, id));
                //const EntityDirty *byte_component = ((const EntityDirty*) ecs_get_id(world, target, id));
                //if (byte_component) zox_logg(" [uknown]")
                if (zoxName) zox_log(" %i: [%s]", zoxName->length, convert_zext_to_text(zoxName->value, zoxName->length))
                else zox_logg(" [tag]")
            }
            /*const long int *value = ((const long int*) ecs_get_id(world, target, id));
            if (value) zox_log(" [%lu]", value)*/
        }
        zox_logg("\n")
    }
}

void set_entity_label_with_zext(ecs_world_t *world, ecs_entity_t e, unsigned char *value, int length) {
    const Children *name_label_children = zox_get(e, Children)
    ecs_entity_t zext_entity = name_label_children->value[0];
    ZextData *zextData = zox_get_mut(zext_entity, ZextData)
    if (zextData->value) free(zextData->value);
    zextData->value = memcpy(malloc(length), value, length);
    zextData->length = length;
    zox_modified(zext_entity, ZextData)
    // zox_set(zext_entity, ZextDirty, { 1 })
    ZextDirty *zextDirty = zox_get_mut(zext_entity, ZextDirty)
    zextDirty->value = 1;
    zox_modified(zext_entity, ZextDirty)
}

void set_entity_label_with_text(ecs_world_t *world, ecs_entity_t e, const char* text) {
    const Children *name_label_children = zox_get(e, Children)
    ecs_entity_t zext_entity = name_label_children->value[0];
    ZextData *zextData = zox_get_mut(zext_entity, ZextData)
    set_zext(zextData, text);
    zox_modified(zext_entity, ZextData)
    // zox_set(zext_entity, ZextDirty, { 1 })
    ZextDirty *zextDirty = zox_get_mut(zext_entity, ZextDirty)
    zextDirty->value = 1;
    zox_modified(zext_entity, ZextDirty)
}

void editor_select_entity(ecs_world_t *world, ecs_entity_t e) {
    if (editor_selected == e) return;
    editor_selected = e;
    // print_entity(world, e);
    if (ecs_is_alive(world, inspector)) {
        const ZoxName *zoxName = zox_get(e, ZoxName)
        const Children *insector_children = zox_get(inspector, Children)
        ecs_entity_t name_label = insector_children->value[2];
        set_entity_label_with_zext(world, name_label, zoxName->value, zoxName->length);
        const ecs_type_t *type = ecs_get_type(world, e);
        const ecs_id_t *type_ids = type->array;
        int32_t i, count = type->count;
        for (i = 0; i < count; i ++) {
            int label_index = 3 + i;
            if (label_index >= insector_children->length) continue;    // limit for now
            ecs_entity_t component_label = insector_children->value[label_index];
            int buffer_index = 0;
            char buffer[32];
            ecs_id_t id = type_ids[i];
            if (ECS_HAS_ID_FLAG(id, PAIR)) {
                ecs_entity_t rel = ecs_pair_first(world, id);
                ecs_entity_t tgt = ecs_pair_second(world, id);
                buffer_index += snprintf(buffer + buffer_index, sizeof(buffer), " pair %s [%lu]", ecs_get_name(world, rel), tgt);
            } else {
                ecs_entity_t comp = id & ECS_COMPONENT_MASK;
                buffer_index += snprintf(buffer + buffer_index, sizeof(buffer), " %s", ecs_get_name(world, comp));
                if (is_component_type_byte(id)) {
                    const EntityDirty *byte_component = ((const EntityDirty*) ecs_get_id(world, e, id));
                    buffer_index += snprintf(buffer + buffer_index, sizeof(buffer), " [%i]", byte_component->value);
                } else if (is_component_type_int(id)) {
                    const ID *int_component = ((const ID*) ecs_get_id(world, e, id));
                    buffer_index += snprintf(buffer + buffer_index, sizeof(buffer), " [%i]", int_component->value);
                } else if (is_component_type_long_int(id)) {
                    const Seed *long_int_component = ((const Seed*) ecs_get_id(world, e, id));
                    buffer_index += snprintf(buffer + buffer_index, sizeof(buffer), " [%lu]", long_int_component->value);
                } else if (is_component_type_int2(id)) {
                    const DraggingDelta *int2_component = ((const DraggingDelta*) ecs_get_id(world, e, id));
                    buffer_index += snprintf(buffer + buffer_index, sizeof(buffer), " [%ix%i]", int2_component->value.x, int2_component->value.y);
                } else {
                    const ZoxName *zoxName = ((const ZoxName*) ecs_get_id(world, e, id));
                    if (zoxName) buffer_index += snprintf(buffer + buffer_index, sizeof(buffer), " len [%i]", zoxName->length);
                    else buffer_index += snprintf(buffer + buffer_index, sizeof(buffer), " [tag]");
                    // buffer_index += snprintf(buffer + buffer_index, sizeof(buffer), " %i: [%s]", zoxName->length, convert_zext_to_text(zoxName->value, zoxName->length))
                }
            }
            set_entity_label_with_text(world, component_label, buffer);
        }
        for (i = count + 3; i < insector_children->length; i++) {
            ecs_entity_t component_label = insector_children->value[i];
            set_entity_label_with_text(world, component_label, "empty");
        }
    }
}

void button_event_hierarchy_clicked(ecs_world_t *world, ecs_entity_t trigger_entity) {
    if (!zox_has(trigger_entity, Children)) return;
    const Children *children = zox_get(trigger_entity, Children)
    ecs_entity_t zext_entity = children->value[0];
    // print_entity_zext(world, zext_entity);
    ecs_entity_t target = zox_get_value(trigger_entity, EntityTarget)
    // zox_log("   > target [%lu]\n", target)
    editor_select_entity(world, target);
}

void HierarchyRefreshSystem(ecs_iter_t *it) {
    // const int font_size = 18 * default_ui_scale;
    ecs_world_t *world = it->world;
    // const int labels_count = 14;
    const unsigned char is_header = 1;
    const unsigned char is_scrollbar = 1;
    const unsigned char list_start = is_header + is_scrollbar;
    const Position2D *position2Ds = ecs_field(it, Position2D, 2);
    const Layer2D *layer2Ds = ecs_field(it, Layer2D, 3);
    const ListUIMax * listUIMaxs = ecs_field(it, ListUIMax, 4);
    const ElementFontSize * elementFontSizes = ecs_field(it, ElementFontSize, 5);
    HierarchyUIDirty *hierarchyUIDirtys = ecs_field(it, HierarchyUIDirty,6);
    PixelSize *pixelSizes = ecs_field(it, PixelSize, 7);
    TextureSize *textureSizes = ecs_field(it, TextureSize, 8);
    Children *childrens = ecs_field(it, Children, 9);
    for (int i = 0; i < it->count; i++) {
        HierarchyUIDirty *hierarchyUIDirty = &hierarchyUIDirtys[i];
        if (!hierarchyUIDirty->value) continue;
        ecs_entity_t e = it->entities[i];
        const Position2D *position2D = &position2Ds[i];
        const Layer2D *layer2D = &layer2Ds[i];
        const ListUIMax *listUIMax = &listUIMaxs[i];
        const ElementFontSize *elementFontSize = &elementFontSizes[i];
        PixelSize *pixelSize = &pixelSizes[i];
        TextureSize *textureSize = &textureSizes[i];
        Children *children = &childrens[i];
        //Children *children = zox_get_mut(e, Children)
        ecs_entity_t canvas = main_canvas;
        int2 canvas_size = ecs_get(world, canvas, PixelSize)->value;
        // spawn extra entities as names
        int elements_visible = listUIMax->value; // zox_get_value(e, ListUIMax)
        int font_size = elementFontSize->value * default_ui_scale;
        unsigned char button_layer = layer2D->value + 1;
        int line_txt_length = 16;
        int2 button_padding = (int2) { (int) (font_size * 0.46f), (int) (font_size * 0.3f) };
        int2 list_margins = (int2) { (int) (font_size * 0.8f), (int) (font_size * 0.8f) };
        int button_inner_margins = (int) (font_size * 0.5f);
        // scrollbar
        ecs_entity_t scrollbar = children->value[1];
        // const int scrollbar_width = 36 * default_ui_scale;
        const int scrollbar_margins = zox_gett_value(scrollbar, ElementMargins).x; // 8 * default_ui_scale;
        const int scrollbar_width = zox_gett_value(scrollbar, PixelSize).x; // 8 * default_ui_scale;
        ClickEvent clicked_element_event = (ClickEvent) { &button_event_hierarchy_clicked };
        text_group_dynamic_array_d* labels = create_text_group_dynamic_array_d();
        ecs_entity_t_array_d* entities = create_ecs_entity_t_array_d();
        add_realm_entity_to_labels(world, local_realm, labels, entities, 0);
        add_entity_to_labels(world, local_terrain, labels, entities, 0);
        int labels_count = labels->size;
        int childrens_length = list_start + labels_count;
        int max_characters = get_max_characters_d("hierarchy", labels);
        int2 window_size = pixelSize->value;
        int2 new_window_size = { (font_size) * max_characters + button_padding.x * 2 + list_margins.x * 2, window_size.y };
        if (is_scrollbar) new_window_size.x += scrollbar_width + scrollbar_margins * 2;
        if (new_window_size.x != window_size.x) {
            window_size = new_window_size;
            pixelSize->value = window_size;
            textureSize->value = window_size;
            on_resized_element(world, e, window_size, int2_to_float2(canvas_size));
        }
        // resize scrollbar
        ecs_entity_t scrollbar_front = zox_gett_value(scrollbar, Children)[0];
        int scrollbar_height = (int) window_size.y * ( float_min(1, (float) listUIMax->value / (float) labels_count));
        int2 scrollbar_size = (int2) { zox_gett_value(scrollbar, PixelSize).x, scrollbar_height };
        zox_set(scrollbar_front, PixelSize, { scrollbar_size })
        zox_set(scrollbar_front, TextureSize, { scrollbar_size })
        on_resized_element(world, scrollbar_front, scrollbar_size, int2_to_float2(canvas_size));
        zox_set(scrollbar_front, DraggableLimits, { (int2) { 0, (window_size.y / 2) - scrollbar_height / 2 } })
        zox_set(scrollbar_front, PixelPosition, { (int2) { 0, (window_size.y - scrollbar_height) / 2 } })
        // refresh elements
        for (int j = list_start; j < children->length; j++) zox_delete(children->value[j]) // destroy previous
        resize_memory_component(Children, children, ecs_entity_t, childrens_length)
        for (int j = 0; j < labels_count; j++) {
            unsigned char render_disabled = !(j >= 0 && j < elements_visible);
            int2 label_position = (int2) { 0, (int) (window_size.y / 2) - (j + 0.5f) * (font_size + button_padding.y * 2) - list_margins.y - j * button_inner_margins };
            if (is_scrollbar) label_position.x -= (scrollbar_width + scrollbar_margins * 2) / 2;
            ecs_entity_t list_element = spawn_button(world, e, label_position, button_padding, float2_half, labels->data[j].text, font_size, button_layer, position2D->value, pixelSize->value, canvas_size, render_disabled);
            zox_set(list_element, ClickEvent, { clicked_element_event.value })
            zox_prefab_set(list_element, EntityTarget, { entities->data[j] })
            children->value[list_start + j] = list_element;
            // zox_log("   > spawned button [%lu] - [%s]\n", list_element, labels->data[j].text)
        }
        for (int j = 0; j < labels_count; j++) free(labels->data[j].text);
        dispose_text_group_dynamic_array_d(labels);
        dispose_ecs_entity_t_array_d(entities);
        hierarchyUIDirty->value = 0;
    }
} zox_declare_system(HierarchyRefreshSystem)
