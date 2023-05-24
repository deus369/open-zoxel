#ifndef zoxel_lines3D
#define zoxel_lines3D

// 0 | EcsOnStore
long int line3D_render_system_id;
long int cube_lines_render_system_id;
zox_declare_tag(Line3D)
zox_component(LineData3D, float6)     //! The two points in a line3D
zox_declare_tag(CubeLines)
zox_component(CubeLinesThickness, float)
#include "prefabs/line3D.c"
#include "shaders/line3D_shader.c"
#include "systems/line3D_render_system.c"
#include "systems/cube_line_render_system.c"
#include "util/terrain_grid.c"

zox_begin_module(Lines3D)
zox_define_tag(Line3D)
zox_define_tag(CubeLines)
zox_define_component(LineData3D)
zox_define_component(CubeLinesThickness)
if (!headless) {
    initialize_shader_line3D();
    zox_system_1(Line3DRenderSystem, 0, [none] Line3D, [in] LineData3D, [in] LineThickness, [in] ColorRGB)
    line3D_render_system_id = ecs_id(Line3DRenderSystem);
    zox_system_1(CubeLineRenderSystem, 0, [none] CubeLines, [in] CubeLinesThickness, [in] ColorRGB, [in] Position3D, [in] Rotation3D, [in] Bounds3D, [in] RenderLod)
    cube_lines_render_system_id = ecs_id(CubeLineRenderSystem);
}
spawn_prefab_line3D(world);
zoxel_end_module(Lines3D)

#endif