void ButtonClickEventSystem(ecs_iter_t *it) {
    zox_iter_world()
    zox_field_in(ClickEvent, clickEvents, 1)
    zox_field_out(ClickState, clickStates, 2)
    zox_field_out(Clicker, clickers, 3)
    for (int i = 0; i < it->count; i++) {
        zox_field_i_out(ClickState, clickStates, clickState)
        if (!clickState->value) continue;
        zox_field_e()
        zox_field_i_in(ClickEvent, clickEvents, clickEvent)
        zox_field_i_out(Clicker, clickers, clicker)
        clickState->value = 0;
        if (clickEvent->value) (*clickEvent->value)(world, clicker->value, e);
    }
} zox_declare_system(ButtonClickEventSystem)
