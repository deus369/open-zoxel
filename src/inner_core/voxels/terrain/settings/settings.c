#define terrain_mode_tiny 1
#define terrain_mode_medium 2
#define terrain_mode_large 3
#define terrain_texture_resolution 32
#define octree_min_height -1.995f // 0.005f
#define noise_positiver2 32000
#define terrain_minus_amplifier 0.0
#ifdef zoxel_on_web
    #define voxels_disable_streaming
#endif
const int2 chunk_texture_size = { terrain_texture_resolution, terrain_texture_resolution };
const unsigned char terrain_min_height = 8;
const int max_chunks_build_per_frame = 32;
const double terrain_frequency2 = 0.004216; // 0.026216
const int terrain_octaves = 12;
const float flat_height_level = -0.56f; // 0.2f;
unsigned char terrain_mode = 0;
unsigned char terrain_spawn_distance;
unsigned char terrain_vertical = 2;
double terrain_amplifier = 64.0;
double terrain_boost = 0.0;
int lowest_voxel_height = -24;
unsigned char high_resolution_terain_lod = 1; // 2 | 1
unsigned char initial_terrain_lod = 2; // 3 | 2
unsigned char lod_division_dividor = 2; // 2 | 3
double terrain_frequency = 0.038216; // 0.026216
uint32_t terrain_seed = 32666;

void set_terrain_render_distance() {
    if (cpu_tier == 3) {
        terrain_spawn_distance = 24;
        terrain_vertical = 3;
        fog_density *= 0.5f;    // .3 for now to cover up transitions
        // set_max_octree_length(5);
    } else if (cpu_tier == 2) {
        terrain_spawn_distance = 18;
        terrain_vertical = 3;
        fog_density *= 0.6f;
    } else if (cpu_tier == 1) {
        terrain_spawn_distance = 8;
        terrain_vertical = 2;
        fog_density *= 0.8f;
    } else {
        terrain_spawn_distance = 4;
        terrain_vertical = 1;
    }
    // preset settings
    if (terrain_mode == terrain_mode_tiny) {
        terrain_spawn_distance = 2;
        terrain_vertical = 1;
    }
    if (terrain_mode == terrain_mode_medium) {
        terrain_spawn_distance = 6;
        terrain_vertical = 1;
    }
    if (terrain_mode == terrain_mode_large) {
        terrain_spawn_distance = 12;
        terrain_vertical = 2;
    }
    #ifdef zox_terrain_preset_large
        terrain_spawn_distance = 8;
        terrain_vertical = 2;
    #endif
    #ifdef zox_terrain_preset_enormous
        terrain_spawn_distance =12;
        terrain_vertical = 3;
    #endif
    terrain_frequency = max_octree_depth * terrain_frequency2; // 0.002216 // 0.008216
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

//#ifndef zox_disable_terrain_octrees
    // double terrain_frequency = 0.028216; // 0.026216
//#else
    //double terrain_frequency = 0.00216; // 0.004216
//#endif
// const double terrain_frequency2 = 0.003216; // 0.026216
// #define zox_terrain_preset_small
// #define zox_terrain_preset_medium // pc can handle this only
// #define zox_terrain_preset_large
// #define zox_terrain_preset_enormous