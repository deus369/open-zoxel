// #include <GL/gl.h>
// https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
// https://stackoverflow.com/questions/12682950/is-there-a-webgl-equivalent-of-glgenvertexarrays-and-glbindvertexarrays
// #include <GLFW/glfw3.h>

int load_all_shaders()
{
    if (load_instance2D_material() != 0)
    {
        printf("Error loading Instance Shader.\n");
    }
    if (load_shader2D_textured() != 0)
    {
        printf("Error loading Texture Shader.\n");
    }
    if (load_shader3D_basic() != 0)
    {
        printf("Error [load_shader3D_basic]\n");
    }
    if (load_shader3D_colored() != 0)
    {
        printf("Error [load_shader3D_colored]\n");
    }
    if (load_shader3D_textured())
    {
        printf("Error [load_shader3D_textured]\n");
    }
    #ifdef zoxel_catch_opengl_errors
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            zoxel_log("GL ERROR with load_all_shaders [%i]\n", err);
            return false;
        }
    #endif
    return 0;
}

//! Cleanup Meshes VBOs
void dispose_opengl()
{
    DisposeInstance2DMaterial();
    dispose_shader2D_textured();
    DisposeInstanced3DMaterial();
    dispose_shader3D_textured();
    dispose_shader3D_colored();
// #ifdef zoxel_catch_opengl_errors
//     GLenum err7 = glGetError();
//     if (err7 != GL_NO_ERROR)
//     {
//         printf("GL ERROR with end of dispose_opengl: %i\n", err7);
//     }
// #endif
}