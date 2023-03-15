#ifndef zoxel_voxels_vox
#define zoxel_voxels_vox

// #define zoxel_debug_vox_read
zoxel_declare_tag(Vox)
#include "util/vox_read_util.c"
#include "prefabs/vox.c"
// #include "systems/noise_chunk_system.c"
int vox_files_count = 0;
vox_file *vox_files;                //! load a bunch of vox files from resources/voxes

void dispose_vox_files() {
    for (int i = 0; i < vox_files_count; i++) {
        dispose_vox_file(&vox_files[i]);
    }
    free(vox_files);
}

void load_vox_files() {
    vox_files_count = 3;
    vox_files = malloc(sizeof(vox_file) * vox_files_count);
    vox_file chicken_vox;
    char* chicken_vox_path = concat_file_path(resources_path, "voxes/monsters/chicken.vox");
    if (read_vox(chicken_vox_path, &chicken_vox) == 0) {
        vox_files[0] = chicken_vox;
    }
    free(chicken_vox_path);
    vox_file vox_slime;
    char* slime_vox_path = concat_file_path(resources_path, "voxes/monsters/slime.vox");
    if (read_vox(slime_vox_path, &vox_slime) == 0) {
        vox_files[1] = vox_slime;
    }
    free(slime_vox_path);
    vox_file vox_mrpenguin;
    char* mrpenguin_vox_path = concat_file_path(resources_path, "voxes/monsters/mrpenguin.vox");
    if (read_vox(mrpenguin_vox_path, &vox_mrpenguin) == 0) {
        vox_files[2] = vox_mrpenguin;
    }
    free(mrpenguin_vox_path);
}

//! The voxels core Sub Module.
void VoxImport(ecs_world_t *world)
{
    zoxel_module(Vox)
    zoxel_define_tag(Vox)
    spawn_prefab_vox(world);
    load_vox_files();
}
#endif