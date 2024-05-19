#ifndef zox_rendering_basics3D
#define zox_rendering_basics3D

#include "shaders/colored3D.c"
#include "shaders/instanced3D.c"
#include "shaders/textured3D.c"
#include "components/material3D.c"
#include "components/material3D_textured.c"
#include "components/material3D_colored.c"
#include "util/instanced3D_material.c"
#include "util/unique3D_material.c"
#include "util/textured3D_shader.c"
#include "util/shader3D_colored.c"
#include "systems/element3D_render_system.c"
#include "systems/render3D_system.c"
#include "systems/render3D_instance_system.c"
#include "systems/render_characters3D_system.c"
#include "systems/mesh_update_system.c"
#include "systems/mesh_update_textured3D_system.c"
#include "systems/mesh_update_characters3D_system.c"

// for now until I move it all to ecs
void rendering_dispose_basic_shaders3D() {
    // dispose_shader3D_colored();
    dispose_shader3D_instance_material();
}

void rendering_load_basic_shaders3D(ecs_world_t *world) {
    if (load_shader3D_basic() != 0) zoxel_log("    ! error loading [shader3D_basic]\n");
    // if (load_shader3D_colored() != 0) zoxel_log("    ! error loading [load_shader3D_colored]\n");
    spawn_material_colored3D(world);
    spawn_material_textured3D(world);
}

void rendering_restore_basic_shaders3D(ecs_world_t *world) {
    // rendering_load_basic_shaders3D(world);
    // restore_shader_textured3D(world);
    // if (load_shader3D_basic() != 0) zoxel_log("    ! error loading [shader3D_basic]\n");
    // if (load_shader3D_colored() != 0) zoxel_log("    ! error loading [load_shader3D_colored]\n");
}

zox_begin_module(RenderingBasics3D)
add_load_shader_function((funfun) { &rendering_load_basic_shaders3D });
add_restore_shader_function((funfun) { &rendering_restore_basic_shaders3D });
zox_define_component(MaterialTextured3D)
zox_define_component(MaterialColored3D)
zox_system_1(InstanceRender3DSystem, 0, [in] Position3D, [in] Rotation3D, [in] Scale1D, [in] Brightness, [none] !MaterialGPULink, [none] !MeshGPULink)
// characters
zox_render3D_system(RenderCharacters3DSystem, [in] MeshIndicies, [in] MeshGPULink, [in] ColorsGPULink, [in] TransformMatrix, [in] RenderDisabled, [none] MeshColorRGBs, [none] !UvsGPULink)
// healthbars
zox_render3D_system(Element3DRenderSystem, [in] TransformMatrix, [in] MeshGPULink, [in] UvsGPULink, [in] ColorsGPULink, [in] MeshIndicies, [in] TextureGPULink, [in] RenderDisabled, [none] rendering.core.SingleMaterial)
// skybox
zox_render3D_system(Render3DSystem, [in] Position3D, [in] Rotation3D, [in] Scale1D, [in] Brightness, [in] MeshGPULink, [in] MaterialGPULink, [in] MeshIndicies, [none] !UvsGPULink, [none] !MeshColorRGBs)
zox_system_1(MeshUpdateSystem, main_thread_pipeline, [out] MeshDirty, [in] MeshIndicies, [in] MeshVertices, [in] MeshGPULink, [in] MaterialGPULink, [none] !MeshUVs, [none] !MeshColorRGBs)
zox_system_1(MeshUpdateTextured3DSystem, main_thread_pipeline, [out] MeshDirty, [in] MeshIndicies, [in] MeshVertices, [in] MeshUVs, [in] MeshColorRGBs, [in] MeshGPULink, [in] UvsGPULink, [in] ColorsGPULink)
zox_system_1(MeshUpdateCharacters3DSystem, main_thread_pipeline, [none] MeshColorRGBs, [out] MeshDirty, [in] MeshIndicies, [in] MeshVertices, [in] MeshColorRGBs, [out] MeshGPULink, [out] ColorsGPULink, [none] !MeshUVs)
zoxel_end_module(RenderingBasics3D)

#endif
