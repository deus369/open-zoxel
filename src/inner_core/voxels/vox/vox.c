#ifndef zoxel_voxels_vox
#define zoxel_voxels_vox

// #define zoxel_debug_vox_read
zoxel_declare_tag(Vox)
#include "util/vox_read_util.c"
#include "prefabs/vox.c"
// #include "systems/noise_chunk_system.c"
int vox_files_count = 0;
vox_file *vox_files;                //! load a bunch of vox files from resources/voxes

void dispose_vox_files()
{
    for (int i = 0; i < vox_files_count; i++)
    {
        dispose_vox_file(&vox_files[i]);
    }
    free(vox_files);
}

//! The voxels core Sub Module.
void VoxImport(ecs_world_t *world)
{
    zoxel_module(Vox)
    zoxel_define_tag(Vox)
    spawn_prefab_vox(world);
    //#ifdef zoxel_test_voxels_terrain
    /*const float model_scale = 0.015f;
    const float distance_to_camera = 0.3f;
    const float side_distance = 0.12f;
    float4 rotation = quaternion_from_euler((float3) { 0, 180 * degreesToRadians, 0 });  // (float4) { 0, -1.6f, 0, -1.0f }
    float3 spawn_position = (float3) { 0, 0, -distance_to_camera };
    #ifdef zoxel_test_voxels_terrain
         spawn_position.y += (1.1f - 0.028f) * overall_voxel_scale; // 2.46f;
    #endif*/

    vox_files_count = 3;
    vox_files = malloc(sizeof(vox_file) * vox_files_count);
    vox_file chicken_vox;
    if (read_vox(resources_folder_name"voxes/monsters/chicken.vox", &chicken_vox) == 0)
    {
        //spawn_vox_from_file(world, &chicken_vox, (float3) { 0.6f, spawn_position.y, spawn_position.z }, rotation, 0.15f);
        // spawn_vox_from_file(world, &chicken_vox, (float3) { -side_distance, spawn_position.y, spawn_position.z }, rotation, model_scale);
        // dispose_vox_file(&chicken_vox); //! Free vox files after use
        vox_files[0] = chicken_vox;
    }
    vox_file vox_slime;
    if (read_vox(resources_folder_name"voxes/monsters/slime.vox", &vox_slime) == 0)
    {
        // spawn_vox_from_file(world, &vox_slime, (float3) { side_distance, spawn_position.y, spawn_position.z }, rotation, model_scale);
        //dispose_vox_file(&vox_slime); //! Free vox files after use
        vox_files[1] = vox_slime;
    }
    vox_file vox_mrpenguin;
    if (read_vox(resources_folder_name"voxes/monsters/mrpenguin.vox", &vox_mrpenguin) == 0)
    {
        // spawn_vox_from_file(world, &vox_mrpenguin, (float3) { 0, spawn_position.y, spawn_position.z }, rotation, model_scale);
        //dispose_vox_file(&vox_mrpenguin); //! Free vox files after use
        vox_files[2] = vox_mrpenguin;
    }
        
    //#endif
}
#endif