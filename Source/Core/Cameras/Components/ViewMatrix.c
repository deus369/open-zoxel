//! Used in OpenGL for rendering. A Camera Frustrum.
typedef struct
{
    float* value;   // float4x4
} ViewMatrix;
ECS_COMPONENT_DECLARE(ViewMatrix);