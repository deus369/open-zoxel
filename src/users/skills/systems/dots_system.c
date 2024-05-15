// DotsSystem
void DotsSystem(ecs_iter_t *it) {
    const float damage_rate = 2.0f;
    const float delta_time = zox_delta_time;
    zox_iter_world()
    zox_field_in(UserLink, userLinks, 1)
    for (int i = 0; i < it->count; i++) {
        zox_field_i_in(UserLink, userLinks, userLink)
        if (!zox_alive(userLink->value)) continue;
        if (zox_gett_value(userLink->value, Dead)) continue;
        const StatLinks *statLinks = zox_get(userLink->value, StatLinks)
        // find health stat, maybe poison should target a stat type directly??
        const ecs_entity_t health_stat = statLinks->value[0];
        StatValue *statValue = zox_get_mut(health_stat, StatValue)
        statValue->value -= delta_time * damage_rate;
        if (statValue->value < 0) statValue->value = 0;
        zox_modified(health_stat, StatValue)
    }
} zox_declare_system(DotsSystem)