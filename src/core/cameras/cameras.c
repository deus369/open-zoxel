#ifndef zoxel_cameras
#define zoxel_cameras

//   todo: Custom name for namespaces used in flecs
//       const char* nm_zoxel_cameras = "Zoxel.cameras";
//       zox_module(world, cameras, nm_zoxel_cameras);

// zoxel_settings
// EcsPreStore | EcsPostUpdate
#define zox_camera_stage EcsPreStore
#include "settings/settings.c"
// shared data
ecs_entity_t main_cameras[max_cameras];
ecs_entity_t ui_cameras[1];
// zoxel_component_declares
zox_declare_tag(Camera)
zox_declare_tag(Camera2D)
zox_declare_tag(Camera3D)
zox_declare_tag(FirstPersonCamera)
zox_declare_tag(CameraFollower2D)
zox_declare_tag(UICamera)
zox_component(ProjectionMatrix, float4x4)
zox_component(ViewMatrix, float4x4)
zox_component(ScreenDimensions, int2)
zox_component(ScreenPosition, int2)
zox_component(FieldOfView, float)
zox_component(CameraNearDistance, float)
zox_entity_component(CameraLink)
zox_entity_component(CameraTarget)
zox_entity_component(CameraFollowLink)
zox_byte_component(FreeRoam)
zox_byte_component(CanFreeRoam)
zox_byte_component(CameraMode)
// zoxel_prefab_includes
#include "prefabs/base_camera.c"
#include "prefabs/camera2D.c"
#include "prefabs/ui_camera.c"
#include "prefabs/free_camera.c"
// zoxel_function_includes
#include "fun/camera_util.c"
// zoxel_system_declares
#include "systems/projection_matrix_system.c"
#include "systems/view_matrix_system.c"
#include "systems/camera2D_follow_system.c"
#include "systems/camera3D_follow_system.c"

void spawn_prefabs_cameras(ecs_world_t *world) {
    spawn_camera_base_prefab(world);
    spawn_prefab_free_camera(world);
    spawn_camera2D_follower_prefab(world);
}

zox_begin_module(Cameras)
// for (int i = 0; i < max_cameras; i++) main_cameras[i] = 0;
// zoxel_component_defines
zox_define_tag(Camera)
zox_define_tag(Camera2D)
zox_define_tag(Camera3D)
zox_define_tag(FirstPersonCamera)
zox_define_tag(CameraFollower2D) // Until fix is found. \todo Fix this, readd it back here where it belongs.
zox_define_tag(UICamera)
zox_define_component(CameraLink)
zox_define_component(CameraTarget)
zox_define_component(ProjectionMatrix)
zox_define_component(ViewMatrix)
zox_define_component(ScreenDimensions)
zox_define_component(ScreenPosition)
zox_define_component(FieldOfView)
zox_define_component(CameraNearDistance)
zox_define_component(FreeRoam)
zox_define_component(CanFreeRoam)
zox_define_component(CameraFollowLink)
zox_define_component(CameraMode)
// zoxel_system_defines
zox_system(Camera2DFollowSystem, EcsPostUpdate, [none] CameraFollower2D, [in] FreeRoam, [in] CameraTarget, [out] Position3D, [out] Rotation3D)
zox_system(Camera3DFollowSystem, EcsPostUpdate, [in] CameraFollowLink, [in] LocalPosition3D, [out] Position3D)
zox_system(ViewMatrixSystem, zox_camera_stage, [in] Position3D, [in] Rotation3D, [in] ProjectionMatrix, [out] ViewMatrix)
zox_system(ProjectionMatrixSystem, zox_camera_stage, [in] ScreenDimensions, [in] FieldOfView, [in] CameraNearDistance, [out] ProjectionMatrix)
zoxel_end_module(Cameras)

#endif