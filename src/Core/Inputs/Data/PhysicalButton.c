//! An entity can have a unique ID generated by guid or something.
typedef struct
{
    bool wasPressedThisFrame;
    bool isPressed;
    bool wasReleasedThisFrame;
} PhysicalButton;