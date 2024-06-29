// GenerateVox == 1
const unsigned char is_generate_vox_airs = 1;
const unsigned char is_generate_vox_outlines = 1;

void vox_outlines(ChunkOctree *chunk, const unsigned char target_depth, byte3 size, const unsigned char black_voxel) {
    byte3 voxel_position;
    for (voxel_position.x = 0; voxel_position.x < size.x; voxel_position.x++) {
        for (voxel_position.y = 0; voxel_position.y < size.y; voxel_position.y++) {
            for (voxel_position.z = 0; voxel_position.z < size.z; voxel_position.z++) {
                unsigned char on_edges = byte3_on_edges(voxel_position, size);
                if (on_edges) {
                    // skip if air
                    byte3 temp_position_2 = voxel_position;
                    if (get_octree_voxel(chunk, &temp_position_2, target_depth) == 0) continue;
                    byte2 set_voxel = (byte2) { black_voxel, target_depth };
                    byte3 temp_position = voxel_position;
                    set_octree_voxel(chunk, &temp_position, &set_voxel, 0);
                }
            }
        }
    }
}

/*void noise_vox(ChunkOctree *chunk, const byte3 size, const unsigned char is_generate_vox_outlines, const byte2 set_voxel_1, const byte2 set_voxel_2, const byte2 set_voxel_3, const byte2 set_voxel_black, const byte2 set_voxel_air) {
    byte3 voxel_position;
    for (voxel_position.x = 0; voxel_position.x < size.x; voxel_position.x++) {
        for (voxel_position.y = 0; voxel_position.y < size.y; voxel_position.y++) {
            for (voxel_position.z = 0; voxel_position.z < size.z; voxel_position.z++) {
                byte2 set_voxel;
                unsigned char did_set_outline = 0;
                if (is_generate_vox_outlines) {
                    unsigned char on_edges = byte3_on_edges(voxel_position, size);
                    if (on_edges) {
                        set_voxel = set_voxel_black;
                        did_set_outline = 1;
                    }
                }
                if (!did_set_outline) {
                    const int rando = rand() % 1000;
                    if (rando <= 150) {
                        const unsigned char on_edge = byte3_on_edge(voxel_position, size);
                        if (!on_edge) set_voxel = set_voxel_1;
                        else set_voxel = set_voxel_air;
                    } else if (rando <= 300) set_voxel = set_voxel_3;
                    else if (rando <= 450) set_voxel = set_voxel_2;
                    else set_voxel = set_voxel_1;
                }
                byte3 node_position = voxel_position;
                set_octree_voxel(chunk, &node_position, &set_voxel, 0);
            }
        }
    }
}*/

void noise_vox2(ChunkOctree *chunk, const unsigned char target_depth, const byte3 size, const byte2 voxel_range, const unsigned char black_voxel) {
    byte3 voxel_position;
    for (voxel_position.x = 0; voxel_position.x < size.x; voxel_position.x++) {
        for (voxel_position.y = 0; voxel_position.y < size.y; voxel_position.y++) {
            for (voxel_position.z = 0; voxel_position.z < size.z; voxel_position.z++) {
                byte3 temp_position = voxel_position;
                if (get_octree_voxel(chunk, &temp_position, target_depth) == black_voxel) continue;
                const int rando = rand() % 1000;
                if (rando > 300) continue;
                const unsigned char voxel = voxel_range.x + rand() % (voxel_range.y - voxel_range.x);
                byte2 set_voxel = (byte2) { voxel, target_depth };
                byte3 node_position = voxel_position;
                set_octree_voxel(chunk, &node_position, &set_voxel, 0);
            }
        }
    }
}

void vox_rubble(ChunkOctree *chunk, const unsigned char target_depth, const byte3 size, const byte2 voxel_range, const int rubble_count) {
    byte3 voxel_position;
    for (int j = 0; j < rubble_count; j++) {
        voxel_position.x = rand() % size.x;
        voxel_position.z = rand() % size.y;
        for (int k = 0; k < size.y; k++) {
            voxel_position.y = k;
            byte3 node_position = voxel_position;
            if (get_octree_voxel(chunk, &node_position, target_depth) == 0) break;
        }
        unsigned char voxel = voxel_range.x + rand() % (voxel_range.y - voxel_range.x);
        byte2 set_voxel = (byte2) { voxel, target_depth };
        byte3 node_position = voxel_position;
        set_octree_voxel(chunk, &node_position, &set_voxel, 0);
    }
}

void GenerateVoxSystem(ecs_iter_t *it) {
    const unsigned char unique_colors = 8;
    const int grass_random = 6;
    // zox_change_check()
    const unsigned char target_depth = max_octree_depth;
    const unsigned char chunk_voxel_length = powers_of_two_byte[target_depth];
    zox_iter_world()
    zox_field_in(Color, colors, 1)
    zox_field_out(GenerateVox, generateVoxs, 2)
    zox_field_out(ChunkOctree, chunkOctrees, 3)
    zox_field_out(ColorRGBs, colorRGBss, 4)
    // zox_field_out(ChunkDirty, chunkDirtys, 5) // for now we don't use this, some models have no dirty / just have data
    for (int i = 0; i < it->count; i++) {
        zox_field_o(GenerateVox, generateVoxs, generateVox)
        if (!generateVox->value) continue;
        zox_field_e()
        zox_field_i(Color, colors, color2)
        zox_field_o(ChunkOctree, chunkOctrees, chunkOctree)
        zox_field_o(ColorRGBs, colorRGBss, colorRGBs)
        const unsigned char colors_count = unique_colors + is_generate_vox_outlines;
        resize_memory_component(ColorRGBs, colorRGBs, color_rgb, colors_count)
        const color_rgb color_rgb_2 = color_to_color_rgb(color2->value);
        const unsigned char black_voxel = unique_colors + 1;
        const byte2 voxel_range = (byte2) { 1, unique_colors - 1 };
        for (int j = 0; j < unique_colors; j++) {
            colorRGBs->value[j] = color_rgb_2;
            color_rgb_multiply_float(&colorRGBs->value[j], 0.7f + 0.6f * (rand() % 100) * 0.01f);
        }
        if (is_generate_vox_outlines) colorRGBs->value[unique_colors] = (color_rgb) { 0, 0, 0 };
        byte3 size = (byte3) { chunk_voxel_length, chunk_voxel_length, chunk_voxel_length };
        fill_new_octree(chunkOctree, 0, target_depth);  // clear chunk
        if (zox_has(e, VoxRubble)) {
            vox_rubble(chunkOctree, target_depth, size, voxel_range, 200);
        } else if (zox_has(e, BlendVox)) {
            const color under_color = zox_get_value(e, SecondaryColor)
            for (int j = colors_count; j < colors_count + unique_colors; j++) {
                color_rgb new_color = color_to_color_rgb(under_color);
                color_rgb_multiply_float(&new_color, 0.7f + 0.6f * (rand() % 100) * 0.01f);
                add_to_ColorRGBs(colorRGBs, new_color);
            }
            const byte2 voxel_range_2 = (byte2) { colors_count + 1, colors_count + unique_colors };
            color_rgb dirt_dark_voxel = color_to_color_rgb(under_color);
            color_rgb_multiply_float(&dirt_dark_voxel, 0.13f);
            add_to_ColorRGBs(colorRGBs, dirt_dark_voxel);
            unsigned char black_voxel_3 = colorRGBs->length;

            voronoi3D(chunkOctree, target_depth, size, voxel_range_2, black_voxel_3);
            noise_vox2(chunkOctree, target_depth, size, voxel_range_2, black_voxel_3);  // avoids black ones

            // create new chunk node here, blend two using mask?
            const unsigned char grass_position = chunk_voxel_length - chunk_voxel_length / 3;
            byte3 voxel_position;
            for (voxel_position.x = 0; voxel_position.x < size.x; voxel_position.x++) {
                for (voxel_position.z = 0; voxel_position.z < size.z; voxel_position.z++) {
                    const unsigned char grass_height = grass_position - rand() % grass_random;
                    for (voxel_position.y = grass_height; voxel_position.y < size.y; voxel_position.y++) {
                        const unsigned char voxel = voxel_range.x + rand() % (voxel_range.y - voxel_range.x);
                        byte2 set_voxel = (byte2) { voxel, target_depth };
                        byte3 temp_position = voxel_position;
                        set_octree_voxel(chunkOctree, &temp_position, &set_voxel, 0);
                    }
                }
            }
            // add outline between blended parts
            byte2 range_blend_1 = voxel_range;
            byte2 range_blend_2 = voxel_range_2;
            range_blend_2.y++;
            // range_blend_2.x--;  // include black
            color_rgb new_color = color_to_color_rgb(color2->value);
            color_rgb_multiply_float(&new_color, 0.13f);
            add_to_ColorRGBs(colorRGBs, new_color);
            unsigned char black_voxel_2 = colorRGBs->length;
            byte2 set_voxel_black_2 = (byte2) { black_voxel_2, target_depth };
            for (voxel_position.x = 0; voxel_position.x < size.x; voxel_position.x++) {
                for (voxel_position.z = 0; voxel_position.z < size.z; voxel_position.z++) {
                    for (voxel_position.y = 1; voxel_position.y < size.y - 1; voxel_position.y++) {
                        byte3 temp_position_1 = voxel_position;
                        const unsigned char voxel = get_octree_voxel(chunkOctree, &temp_position_1, target_depth);
                        byte3 temp_position_up = byte3_up(voxel_position);
                        const unsigned char voxel_up = get_octree_voxel(chunkOctree, &temp_position_up, target_depth);
                        if ((voxel >= range_blend_1.x && voxel <= range_blend_1.y && voxel_up >= range_blend_2.x && voxel_up <= range_blend_2.y) || (voxel >= range_blend_2.x && voxel <= range_blend_2.y && voxel_up >= range_blend_1.x && voxel_up <= range_blend_1.y)) {
                            byte3 temp_position = voxel_position;
                            set_octree_voxel(chunkOctree, &temp_position, &set_voxel_black_2, 0);
                        }
                        byte3 temp_position_down = byte3_down(voxel_position);
                        const unsigned char voxel_down = get_octree_voxel(chunkOctree, &temp_position_down, target_depth);
                        if ((voxel >= range_blend_1.x && voxel <= range_blend_1.y && voxel_down >= range_blend_2.x && voxel_down <= range_blend_2.y) || (voxel >= range_blend_2.x && voxel <= range_blend_2.y && voxel_down >= range_blend_1.x && voxel_down <= range_blend_1.y)) {
                            byte3 temp_position = voxel_position;
                            set_octree_voxel(chunkOctree, &temp_position, &set_voxel_black_2, 0);
                        }
                        if (voxel_position.x != size.x - 1) {
                            byte3 temp_position_right = byte3_right(voxel_position);
                            const unsigned char voxel_right = get_octree_voxel(chunkOctree, &temp_position_right, target_depth);
                            if ((voxel >= range_blend_1.x && voxel <= range_blend_1.y && voxel_right >= range_blend_2.x && voxel_right <= range_blend_2.y) || (voxel >= range_blend_2.x && voxel <= range_blend_2.y && voxel_right >= range_blend_1.x && voxel_right <= range_blend_1.y)) {
                                byte3 temp_position = voxel_position;
                                set_octree_voxel(chunkOctree, &temp_position, &set_voxel_black_2, 0);
                            }
                        }
                        if (voxel_position.x != 0) {
                            byte3 temp_position_left = byte3_left(voxel_position);
                            const unsigned char voxel_left  = get_octree_voxel(chunkOctree, &temp_position_left, target_depth);
                            if ((voxel >= range_blend_1.x && voxel <= range_blend_1.y && voxel_left  >= range_blend_2.x && voxel_left  <= range_blend_2.y) || (voxel >= range_blend_2.x && voxel <= range_blend_2.y && voxel_left  >= range_blend_1.x && voxel_left <= range_blend_1.y)) {
                                byte3 temp_position = voxel_position;
                                set_octree_voxel(chunkOctree, &temp_position, &set_voxel_black_2, 0);
                            }
                        }
                        if (voxel_position.z != size.z - 1) {
                            byte3 temp_position_front = byte3_front(voxel_position);
                            const unsigned char voxel_front = get_octree_voxel(chunkOctree, &temp_position_front, target_depth);
                            if ((voxel >= range_blend_1.x && voxel <= range_blend_1.y && voxel_front >= range_blend_2.x && voxel_front <= range_blend_2.y) || (voxel >= range_blend_2.x && voxel <= range_blend_2.y && voxel_front >= range_blend_1.x && voxel_front <= range_blend_1.y)) {
                                byte3 temp_position = voxel_position;
                                set_octree_voxel(chunkOctree, &temp_position, &set_voxel_black_2, 0);
                            }
                        }
                        if (voxel_position.z != 0) {
                            byte3 temp_position_back = byte3_back (voxel_position);
                            const unsigned char voxel_back  = get_octree_voxel(chunkOctree, &temp_position_back, target_depth);
                            if ((voxel >= range_blend_1.x && voxel <= range_blend_1.y && voxel_back >= range_blend_2.x && voxel_back <= range_blend_2.y) || (voxel >= range_blend_2.x && voxel <= range_blend_2.y && voxel_back  >= range_blend_1.x && voxel_back  <= range_blend_1.y)) {
                                byte3 temp_position = voxel_position;
                                set_octree_voxel(chunkOctree, &temp_position, &set_voxel_black_2, 0);
                            }
                        }
                    }
                }
            }
        } else {
            color_rgb dirt_dark_voxel = color_to_color_rgb(color2->value);
            color_rgb_multiply_float(&dirt_dark_voxel, 0.13f);
            add_to_ColorRGBs(colorRGBs, dirt_dark_voxel);
            unsigned char black_voxel_3 = colorRGBs->length;
            voronoi3D(chunkOctree, target_depth, size, voxel_range, black_voxel_3);
            noise_vox2(chunkOctree, target_depth, size, voxel_range, black_voxel_3);
        }
        if (is_generate_vox_outlines) vox_outlines(chunkOctree, target_depth, size, black_voxel);
#ifndef zox_disable_closing_octree_nodes
        close_same_nodes(chunkOctree);
#endif
        generateVox->value = 0;
    }
} zox_declare_system(GenerateVoxSystem)

/*byte2 set_voxel;
int rando = rand() % 1000;
if (rando <= 200) set_voxel = set_voxel_1;
else if (rando <= 400) set_voxel = set_voxel_2;
else set_voxel = set_voxel_3;
byte3 temp_position = voxel_position;
set_octree_voxel(chunkOctree, &temp_position, &set_voxel, 0);*/

/*for (voxel_position.x = 0; voxel_position.x < size.x; voxel_position.x++) {
    for (voxel_position.y = 0; voxel_position.y < size.y; voxel_position.y++) {
        for (voxel_position.z = 0; voxel_position.z < size.z; voxel_position.z++) {
            byte2 set_voxel;
            unsigned char did_set_outline = 0;
            if (is_generate_vox_outlines) {
                unsigned char on_edges = byte3_on_edges(voxel_position, size);
                if (on_edges) {
                    set_voxel = set_voxel_black;
                    did_set_outline = 1;
                }
            }
            if (!did_set_outline) {
                const int rando = rand() % 1000;
                if (rando <= 150) {
                    const unsigned char on_edge = byte3_on_edge(voxel_position, size);
                    if (!on_edge) set_voxel = set_voxel_1;
                    else set_voxel = set_voxel_air;
                } else if (rando <= 300) set_voxel = set_voxel_3;
                else if (rando <= 450) set_voxel = set_voxel_2;
                else set_voxel = set_voxel_1;
            }
            byte3 node_position = voxel_position;
            set_octree_voxel(chunkOctree, &node_position, &set_voxel, 0);
        }
    }
}*/

// testing
/*if (voxel_position.x >= chunk_voxel_length / 2) set_voxel = set_voxel_1;
else set_voxel = set_voxel_2;*/

            /*color_rgb color_4 = color_to_color_rgb(under_color);
            color_rgb color_5 = color_4;
            color_rgb_multiply_float(&color_5, 0.7f);
            color_rgb color_6 = color_4;
            color_rgb_multiply_float(&color_6, 1.2f);
            add_to_ColorRGBs(colorRGBs, color_4);
            add_to_ColorRGBs(colorRGBs, color_5);
            add_to_ColorRGBs(colorRGBs, color_6);
            byte2 set_voxel_4 = (byte2) { colors_count + 1, target_depth };
            byte2 set_voxel_5 = (byte2) { colors_count + 2, target_depth };
            byte2 set_voxel_6 = (byte2) { colors_count + 3, target_depth };*/
