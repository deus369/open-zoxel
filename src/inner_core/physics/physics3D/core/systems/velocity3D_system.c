void Velocity3DSystem(ecs_iter_t *it) { //! Veloco Rapters!
    #ifdef zoxel_disable_velocity
        return;
    #endif
    double delta_time = zoxel_delta_time;
    Position3D *positions = ecs_field(it, Position3D, 1);
    const Velocity3D *velocity3Ds = ecs_field(it, Velocity3D, 2);
    for (int i = 0; i < it->count; i++) {
        const Velocity3D *velocity3D = &velocity3Ds[i];
        Position3D *position = &positions[i];
        position->value.x += velocity3D->value.x * delta_time;
        position->value.y += velocity3D->value.y * delta_time;
        position->value.z += velocity3D->value.z * delta_time;
    }
}
zoxel_declare_system(Velocity3DSystem)