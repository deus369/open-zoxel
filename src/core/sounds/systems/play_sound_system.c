//! Plays sounds on main thread.
void PlaySoundSystem(ecs_iter_t *it) {
    ecs_world_t *world = it->world;
    const TriggerSound *playSounds = ecs_field(it, TriggerSound, 2);
    const SoundLength *soundLengths = ecs_field(it, SoundLength, 3);
    const SDLSound *sdlSounds = ecs_field(it, SDLSound, 4);
    for (int i = 0; i < it->count; i++) {
        const TriggerSound *triggerSound = &playSounds[i];
        if (triggerSound->value != 1) continue;
        const SDLSound *sdlSound = &sdlSounds[i];
        const SoundLength *soundLength = &soundLengths[i];
        if (sdlSound->value != NULL) {
            if (Mix_PlayChannel(-1, sdlSound->value, 0) == -1) {
                zoxel_log(" - playing sound failed [%s]\n", Mix_GetError());
                zox_delete(it->entities[i])
            } else {
                zox_set_only(it->entities[i], DestroyInTime, { soundLength->value })
            }
        }
    }
}
zox_declare_system(PlaySoundSystem);