void emit_particle3Ds(ecs_world_t *world, float3 spawn_position, int spawn_count) {
    float2 positionBounds = { 0.1f, 0.2f };
    const float2 velocityBounds = { 0.03f, 0.2f };
    const float2 scaleBounds = { 0.02f, 0.13f };
    const double2 lifeTime = { 0.1f, 2.0f };
    const float2 brightnessBounds = { 0.2f, 0.8f };
    // Create a SpaceShip prefab with a Defense component.
    Position3D *positions = malloc(sizeof(Position3D) * spawn_count);
    Velocity3D *velocity3Ds = malloc(sizeof(Velocity3D) * spawn_count);
    // Rotation3D *rotations = malloc(sizeof(Rotation3D) * spawn_count);
    // Acceleration *accelerations = malloc(sizeof(Acceleration) * spawn_count);
    // Torque3D *torques = malloc(sizeof(Torque3D) * spawn_count);
    Scale1D *scale1Ds = malloc(sizeof(Scale1D) * spawn_count);
    Brightness *brightnesses = malloc(sizeof(Brightness) * spawn_count);
    DestroyInTime *destroyInTimes = malloc(sizeof(DestroyInTime) * spawn_count);
    for (int i = 0; i < spawn_count; i++) {
        positions[i].value = (float3) {
            ((rand() % 101) / 100.0f) * positionBounds.y - (positionBounds.y / 2.0f),
            ((rand() % 101) / 100.0f) * positionBounds.y - (positionBounds.y / 2.0f),
            ((rand() % 101) / 100.0f) * positionBounds.y - (positionBounds.y / 2.0f)
        };
        positions[i].value.x += spawn_position.x;
        positions[i].value.y += spawn_position.y;
        positions[i].value.z += spawn_position.z;
        // velocity
        velocity3Ds[i].value = (float3) {
            ((rand() % 101) / 100.0f) * 1.0f - 0.5f,
            ((rand() % 101) / 100.0f) * 1.0f - 0.5f,
            ((rand() % 101) / 100.0f) * 1.0f - 0.5f
        };
        velocity3Ds[i].value = float3_normalize(velocity3Ds[i].value);
        velocity3Ds[i].value = float3_multiply_float(velocity3Ds[i].value, (velocityBounds.y - velocityBounds.x));
        if (velocity3Ds[i].value.x < 0) velocity3Ds[i].value.x -= velocityBounds.x;
        else velocity3Ds[i].value.x += velocityBounds.x;
        if (velocity3Ds[i].value.y < 0) velocity3Ds[i].value.y -= velocityBounds.x;
        else velocity3Ds[i].value.y += velocityBounds.x;
        if (velocity3Ds[i].value.z < 0) velocity3Ds[i].value.z -= velocityBounds.x;
        else velocity3Ds[i].value.z += velocityBounds.x;
        // torques[i].value = ((rand() % 101) / 100.0f) * torqueBounds - (torqueBounds / 2.0f);
        scale1Ds[i].value = scaleBounds.x + ((rand() % 101) / 100.0f) * (scaleBounds.y - scaleBounds.x);
        brightnesses[i].value = brightnessBounds.x + ((rand() % 101) / 100.0f) * (brightnessBounds.y - brightnessBounds.x);
        destroyInTimes[i].value = lifeTime.x + ((rand() % 101) / 100.0f) *  (lifeTime.y - lifeTime.x);
    }
    // const ecs_entity_t *particlesArray = 
    ecs_bulk_init(world, &(ecs_bulk_desc_t) {
        .count = spawn_count,
        .ids = {
            ecs_pair(EcsIsA, prefab_particle3D),
            ecs_id(Position3D),
            ecs_id(Rotation3D),
            ecs_id(Scale1D),
            ecs_id(Velocity3D),
            ecs_id(Acceleration3D),
            // ecs_id(Torque3D),
            ecs_id(Brightness),
            ecs_id(DestroyInTime)
        },
        // provide data for each id
        .data = (void*[]) {       
            NULL,           // Prefab pair, what is it used for?
            positions,
            NULL,           // Rotation3D
            scale1Ds,
            velocity3Ds,
            NULL,           // Acceleration3D
            // NULL,        // Torque3D
            brightnesses,
            destroyInTimes
        }
    });
    free(positions);
    free(scale1Ds);
    free(velocity3Ds);
    // free(torques);
    free(brightnesses);
    free(destroyInTimes);
}

void emit_particle3Ds_slow(ecs_world_t *world, float3 emit_position, int spawn_count) {
    float3 spawn_bounds = { 0.4f, 0.8, 0.4f };
    float3 acceleration3D_bounds = { 0.4f, 0.8, 0.4f };
    for (int i = 0; i < spawn_count; i++) {
        // ecs_entity_t e =
        float3 spawn_position = emit_position;
        float3 spawn_acceleration3D = float3_zero;
        float3_add_float3_p(&spawn_position, (float3) {
            ((rand() % 101) / 100.0f) * spawn_bounds.x - (spawn_bounds.x / 2.0f),
            ((rand() % 101) / 100.0f) * spawn_bounds.y - (spawn_bounds.y / 2.0f),
            ((rand() % 101) / 100.0f) * spawn_bounds.z - (spawn_bounds.z / 2.0f)
        });
        float3_add_float3_p(&spawn_acceleration3D, (float3) {
            ((rand() % 101) / 100.0f) * acceleration3D_bounds.x - (acceleration3D_bounds.x / 2.0f),
            ((rand() % 101) / 100.0f) * acceleration3D_bounds.y - (acceleration3D_bounds.y / 2.0f),
            ((rand() % 101) / 100.0f) * acceleration3D_bounds.z - (acceleration3D_bounds.z / 2.0f)
        });
        spawn_particle3D(world, spawn_position, spawn_acceleration3D);
    }

}

void Particle3DEmitSystem(ecs_iter_t *it) {
    ecs_world_t *world = it->world;
    const Position3D *position3Ds = ecs_field(it, Position3D, 2);
    const ParticleEmitRate *particleEmitRates = ecs_field(it, ParticleEmitRate, 3);
    for (int i = 0; i < it->count; i++) {
        const Position3D *position3D = &position3Ds[i];
        const ParticleEmitRate *particleEmitRate = &particleEmitRates[i];
        emit_particle3Ds_slow(world, position3D->value, particleEmitRate->value);
    }
} zox_declare_system(Particle3DEmitSystem)