typedef struct {
    PhysicalButton start;
    PhysicalButton select;
    PhysicalButton a;
    PhysicalButton b;
    PhysicalButton x;
    PhysicalButton y;
    PhysicalButton lb;
    PhysicalButton rb;
    PhysicalButton lt;
    PhysicalButton rt;
    PhysicalButton left_stick_push;
    PhysicalButton right_stick_push;
    PhysicalStick left_stick;
    PhysicalStick right_stick;
} Gamepad;
zox_custom_component(Gamepad)

#define gamepad_zero {\
    .start = (PhysicalButton) { 0, 0, 0 },\
    .select = (PhysicalButton) { 0, 0, 0 },\
    .a = (PhysicalButton) { 0, 0, 0 },\
    .b = (PhysicalButton) { 0, 0, 0 },\
    .x = (PhysicalButton) { 0, 0, 0 },\
    .y = (PhysicalButton) { 0, 0, 0 },\
    .lb = (PhysicalButton) { 0, 0, 0 },\
    .rb = (PhysicalButton) { 0, 0, 0 },\
    .lt = (PhysicalButton) { 0, 0, 0 },\
    .rt = (PhysicalButton) { 0, 0, 0 },\
    .left_stick_push = (PhysicalButton) { 0, 0, 0 },\
    .right_stick_push = (PhysicalButton) { 0, 0, 0 },\
    .left_stick = (PhysicalStick) { .value = float2_zero },\
    .right_stick = (PhysicalStick) { .value = float2_zero }\
}

unsigned char gamepad_is_any_input(const Gamepad *gamepad) {
    // zoxel_log(" > gamepad left_stick [%fx%f]\n", gamepad->left_stick.value.x, gamepad->left_stick.value.y);
    // zoxel_log(" > gamepad right_stick [%fx%f]\n", gamepad->right_stick.value.x, gamepad->right_stick.value.y);
    return gamepad->start.is_pressed ||
        gamepad->select.is_pressed ||
        gamepad->a.is_pressed ||
        gamepad->b.is_pressed ||
        gamepad->x.is_pressed ||
        gamepad->y.is_pressed ||
        gamepad->lb.is_pressed ||
        gamepad->rb.is_pressed ||
        gamepad->lt.is_pressed ||
        gamepad->rt.is_pressed ||
        gamepad->left_stick_push.is_pressed ||
        gamepad->right_stick_push.is_pressed ||
        float_abs(gamepad->left_stick.value.x) >= joystick_min_cutoff ||
        float_abs(gamepad->left_stick.value.y) >= joystick_min_cutoff ||
        float_abs(gamepad->right_stick.value.x) >= joystick_min_cutoff ||
        float_abs(gamepad->right_stick.value.y) >= joystick_min_cutoff; 
}