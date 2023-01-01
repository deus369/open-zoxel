//! A basic x2 building block.
typedef struct
{
    float x;
    float y;
    float z;
} float3;

void float3_print(const float3 input)
{
    zoxel_log("    Float3 [%f %f %f]\n", input.x, input.y, input.z);
}

float3 float3_multiply_float(float3 input, float mul) // by-value param invokes class copy-ctor
{
    input.x *= mul;
    input.y *= mul;
    input.z *= mul;
    return input;
}

float3 float3_add(float3 inputA, float3 inputB) // by-value param invokes class copy-ctor
{
    return (float3) { inputA.x + inputB.x, inputA.y + inputB.y, inputA.z + inputB.z };
}

float3 float3_divide_float(float3 input, float div) // by-value param invokes class copy-ctor
{
    return (float3) { input.x / div, input.y / div, input.z / div };
}

float3 float3_normalize(float3 input)
{
    float length = sqrt(input.x * input.x + input.y * input.y + input.z * input.z);
    return float3_divide_float(input, length);
}

//! Fixed from https://www.mathsisfun.com/algebra/vectors-cross-product.html
float3 float3_cross(float3 a, float3 b)
{
    return (float3) { 
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}