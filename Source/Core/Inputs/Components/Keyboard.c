//! An entity can have a unique ID generated by guid or something.
/**
*   \todo Use a hashmap with keys keyboard->keys['a'] for more dynamic access.
*/
typedef struct
{
    PhysicalButton enter;
    PhysicalButton space;
    PhysicalButton escape;
    PhysicalButton backspace;
    PhysicalButton leftControl;
    PhysicalButton rightControl;
    PhysicalButton leftShift;
    PhysicalButton rightShift;
    PhysicalButton tab;
    PhysicalButton capsLock;
    PhysicalButton a;
    PhysicalButton b;
    PhysicalButton c;
    PhysicalButton d;
    PhysicalButton e;
    PhysicalButton f;
    PhysicalButton g;
    PhysicalButton h;
    PhysicalButton i;
    PhysicalButton j;
    PhysicalButton k;
    PhysicalButton l;
    PhysicalButton m;
    PhysicalButton n;
    PhysicalButton o;
    PhysicalButton p;
    PhysicalButton q;
    PhysicalButton r;
    PhysicalButton s;
    PhysicalButton t;
    PhysicalButton u;
    PhysicalButton v;
    PhysicalButton w;
    PhysicalButton x;
    PhysicalButton y;
    PhysicalButton z;
    PhysicalButton _0;
    PhysicalButton _1;
    PhysicalButton _2;
    PhysicalButton _3;
    PhysicalButton _4;
    PhysicalButton _5;
    PhysicalButton down;
    PhysicalButton up;
    PhysicalButton left;
    PhysicalButton right;
} Keyboard;
ECS_COMPONENT_DECLARE(Keyboard);