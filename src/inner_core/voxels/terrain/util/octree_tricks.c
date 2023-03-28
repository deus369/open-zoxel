// Check all voxels on a side, instead of just one, a big voxel with 4 small voxels on its side should be face culled.
// max_depth is per chunk... refactor that
// Fix issues between chunks of different levels of division
// function to check all adjacent voxels are solid on the face
unsigned char is_adjacent_all_solid(unsigned char direction, const ChunkOctree *root_node, const ChunkOctree *parent_node, const ChunkOctree *neighbors[],
    int3 octree_position, int3 node_position, unsigned char depth, unsigned char max_depth, unsigned char neighbors_max_depths[]) {
    unsigned char chunk_index = 0;
    const ChunkOctree *adjacent_node = find_adjacent_ChunkOctree(root_node, parent_node, octree_position, node_position, depth, direction, neighbors, &chunk_index);
    if (adjacent_node == NULL || adjacent_node->value == 0) {
        if (adjacent_node == NULL && chunk_index != 0) {
            #ifdef voxels_hide_terrain_edge
                return 1;
            #else
                return 0;
            #endif
        }
        return 0;
    } else if (adjacent_node->nodes && ((chunk_index == 0 && depth < max_depth) || 
        (chunk_index != 0 && depth < neighbors_max_depths[chunk_index - 1]))) {
        depth++;
        int3_multiply_int_p(&octree_position, 2);
        for (unsigned char i = 0; i < octree_length; i++) {
            int3 local_position = octree_positions[i];
            if (direction == direction_left) {
                if (local_position.x != 0) {
                    continue;
                }
            } else if (direction == direction_right) {
                if (local_position.x != 1) {
                    continue;
                }
            } else if (direction == direction_down) {
                if (local_position.y != 0) {
                    continue;
                }
            } else if (direction == direction_up) {
                if (local_position.y != 1) {
                    continue;
                }
            } else if (direction == direction_back) {
                if (local_position.z != 0) {
                    continue;
                }
            } else if (direction == direction_front) {
                if (local_position.z != 1) {
                    continue;
                }
            } else {
                continue;
            }
            // check underneath nodes
            int3 new_octree_position = int3_add(octree_position, local_position);
            unsigned char is_adjacent_solid = is_adjacent_all_solid(direction, root_node, &adjacent_node->nodes[i], neighbors, new_octree_position, local_position, depth, max_depth, neighbors_max_depths);
            if (is_adjacent_solid == 0) {
                return 0;
            }
        }
    }
    return 1;
}

#define zoxel_octree_check(direction_name)\
    if (!is_adjacent_all_solid(direction##_##direction_name, root_node, parent_node, neighbors,\
        octree_position, node_position, depth, max_depth, neighbors_max_depths))

unsigned char get_max_depth_from_division(unsigned char chunk_division) {
    #ifdef zoxel_voxel_disable_distance_division
        unsigned char max_depth = max_octree_depth;
    #else
        unsigned char depth_addition = chunk_division / 2;
        unsigned char max_depth;
        if (depth_addition < inner_render_buffer) {
            max_depth = max_octree_depth;
        } else if ((depth_addition - inner_render_buffer) > max_octree_depth) {
            max_depth = 0;
        } else {
            max_depth = max_octree_depth - (depth_addition - inner_render_buffer);
        }
    #endif
    return max_depth;
}