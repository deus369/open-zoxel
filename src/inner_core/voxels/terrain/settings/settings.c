int terrain_spawn_distance;
int terrain_vertical = 2;
const unsigned char terrain_min_height = 8;
double terrain_amplifier = 64.0;
double terrain_boost = 0.0;
int lowest_voxel_height = -24;
int inner_render_buffer = 1;
int lod_division_dividor = 3;
const int max_chunks_build_per_frame = 32;
#define terrain_texture_resolution 32 // 16
const int2 chunk_texture_size = { terrain_texture_resolution, terrain_texture_resolution };
#ifndef WEB_BUILD
#else
    #define voxels_disable_streaming
#endif
#define octree_min_height -1.995f // 0.005f
#define noise_positiver2 32000
#define terrain_minus_amplifier 0.0
#ifdef voxel_octrees
    double terrain_frequency = 0.028216; // 0.026216
#else
    double terrain_frequency = 0.00216; // 0.004216
#endif
const int terrain_octaves = 12;
uint32_t terrain_seed = 32666;
const float flat_height_level = -0.56f; // 0.2f;

void set_terrain_render_distance() {
    if (cpu_tier == 3) {
        terrain_spawn_distance = 18; // 24;
        terrain_vertical = 3;
        lod_division_dividor = 5;
        fog_density *= 0.36f;    // .3 for now to cover up transitions
        // set_max_octree_length(5);
    } else if (cpu_tier == 2) {
        terrain_spawn_distance = 12;
        fog_density *= 0.75f;
    } else if (cpu_tier == 1) {
        terrain_spawn_distance = 6;
        fog_density *= 0.9f;
    } else {
        terrain_spawn_distance = 4;
    }
    terrain_frequency = max_octree_depth * 0.002216; // 0.008216
    terrain_boost = 0; // -8 * terrain_vertical; //  + max_octree_depth * 4;
    terrain_amplifier = 16 + terrain_vertical * 16;
    lowest_voxel_height = - (terrain_vertical) * 16 + 1;
    terrain_seed = get_unique_time_seed();
    #ifdef zoxel_debug_settings
        zoxel_log(" > terrain settings set\n");
        zoxel_log("     + terrain seed is [%i]\n", (int) terrain_seed);
        zoxel_log("     + octree depth is [%i]\n", max_octree_depth);
        zoxel_log("     + render distance is [%i]\n", terrain_spawn_distance);
        zoxel_log("     + terrain vertical is [%i]\n", terrain_vertical);
        zoxel_log("     + lod dividor is [%i]\n", lod_division_dividor);
        zoxel_log("     + height amplifier is [%d]\n", terrain_amplifier);
        zoxel_log("     + height noise frequency is [%d]\n", terrain_frequency);
        zoxel_log("     + height boost is [%d]\n", terrain_boost);
        zoxel_log("     + lowest low is [%d]\n", lowest_voxel_height);
    #endif
}