#ifndef zox_rendering_basics2D
#define zox_rendering_basics2D

#include "data/settings.c"
#include "shaders/instanced2D.c"
#include "shaders/textured2D.c"
#include "components/material_textured2D.c"
#include "util/square_mesh.c"
#include "util/material2D.c"
#include "util/instanced2D_material.c"
#include "util/render_util.c"
#include "util/shader_textured2D.c"
#include "systems/mesh2D_update_system.c"
#include "systems/mesh2D_uvs_update_system.c"
#include "systems/render2D_instance_system.c"
#include "systems/render2D_system.c"

// for now until I move it all to ecs
void rendering_dispose_basic_shaders2D() {
    dispose_shader2D_instance_material();
    dispose_shader2D_textured();
}

void spawn_shaders_basics2D(ecs_world_t *world) {
    spawn_material_textured2D(world);
    load_instance2D_material();
}

zox_begin_module(RenderingBasics2D)
add_load_shader_function(&spawn_shaders_basics2D);
zox_define_component(MaterialTextured2D)
zox_render3D_system(RenderMaterial2DSystem, [in] Position2D, [in] Rotation2D, [in] Scale1D, [in] Brightness, [in] MaterialGPULink, [in] TextureGPULink, [none] !MeshGPULink)
zox_system_1(Mesh2DUvsUpdateSystem, main_thread_pipeline, [out] MeshDirty, [in] MeshIndicies, [in] MeshVertices2D, [in] MeshUVs, [in] MeshGPULink, [in] UvsGPULink, [none] !MeshColorRGBs)
zox_system_1(Mesh2DUpdateSystem, main_thread_pipeline, [out] MeshDirty, [in] MeshIndicies, [in] MeshVertices2D, [in] MeshGPULink, [in] MaterialGPULink, [none] !MeshUVs, [none] !MeshColorRGBs)
zoxel_end_module(RenderingBasics2D)

#endif
