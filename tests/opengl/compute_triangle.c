// cc -std=c99 tests/opengl/compute_triangle.c -o compute_triangle -lGL -lGLEW -lglfw && ./compute_triangle
// sudo apt-get install libglew-dev libglfw3-dev
// sudo pacman -S glew glfw-x11
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../../src/core/util/logs.c"
#include "../../src/core/rendering/opengl/util/error_util.c"
#include "../../src/core/rendering/opengl/util/compute_util.c"
#include "glfw_util.c"

typedef struct
{
    float x;
    float y;
    float z;
} vec3;

const int vertex_count = 3;
const int single_data_length = 3 * 4;    // 24 * 16
const int data_length = vertex_count * 3 * 4;    // 24 * 16
GLuint position_buffer, vbo, shader_program;
GLuint compute_shader, compute_program;
const char* vertex_shader_source =
    "#version 300 es\n"
    "in lowp vec4 position;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = position;\n"
    "}\n\0";
const char* fragment_shader_source =
    "#version 300 es\n"
    "out lowp vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.32f, 0.16f, 0.16f, 1.0f);\n"
    "}\n\0";
const char* compute_shader_source = "\
#version 310 es\n\
\
struct vec3z\
{\
    float x;\
    float y;\
    float z;\
};\
\
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;\
layout(std430, binding = 0) buffer PositionBuffer {\
    vec3z positions[];\
};\
\
void main() {\
    int index = int(gl_WorkGroupID.x);\
    vec3z position;\
    float size = 0.98;\
    if (index == 0) {\
        position = vec3z(-size, -size, 0.0);\
    } else if (index == 1) {\
        position = vec3z(size, -size, 0.0);\
    } else if (index == 2) {\
        position = vec3z(0.0, size, 0.0);\
    }\
    positions[index] = position;\
}";

GLuint create_shader_program() {
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    check_opengl_error("create_shader_program");
    return shader_program;
}

// position buffer used for vertex positions
GLuint setup_position_buffer() {
    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, position_buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, data_length, NULL, GL_DYNAMIC_DRAW);
    check_opengl_error("setup_position_buffer");
    return position_buffer;
}

GLuint create_vertex_buffer(GLuint shader_program, GLuint position_buffer) {
    // GLuint position_attrib = 0; 
    GLuint position_attrib = glGetAttribLocation(shader_program, "position");
    printf("    > position attrib [%i]\n", position_attrib);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(position_attrib);
    // Copy vertex data to buffer
    glBufferData(GL_ARRAY_BUFFER, data_length, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_COPY_WRITE_BUFFER, position_buffer);
    glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ARRAY_BUFFER, 0, 0, data_length);
    // Set up vertex attributes
    glVertexAttribPointer(position_attrib, vertex_count, GL_FLOAT, GL_FALSE, single_data_length, (void*) 0);
    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    check_opengl_error("create_vertex_buffer");
    return vbo;
}

// Set up compute shader
void setup_compute_buffer(GLuint position_buffer) {
    compute_shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute_shader, 1, &compute_shader_source, NULL);
    glCompileShader(compute_shader);
    compute_program = glCreateProgram();
    glAttachShader(compute_program, compute_shader);
    glLinkProgram(compute_program);
    check_opengl_error("setup_compute_buffer");
}

void attach_buffer_to_compute_program() {
    glUseProgram(compute_program);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, position_buffer);
    glUseProgram(0);
    check_opengl_error("attach_buffer_to_compute_program");
}

// Dispatch compute shader to generate vertex positions
void run_compute_shader(GLuint compute_program) {
    printf("    > Running compute\n");
    glUseProgram(compute_program);
    glDispatchCompute(vertex_count, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glUseProgram(0);
    check_opengl_error("run_compute_shader");
}

void cleanup()
{
    glDeleteShader(compute_shader);
    glDeleteProgram(compute_program);
    glDeleteBuffers(1, &position_buffer);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);
}

int main()
{
    GLFWwindow* window = open_glfw_window(1, 0);
    int supports_compute = check_compute_shader_support();
    if (supports_compute)
    {
        position_buffer = setup_position_buffer();
        setup_compute_buffer(position_buffer);
        attach_buffer_to_compute_program();
        run_compute_shader(compute_program);
        // print_buffer(position_buffer);
        shader_program = create_shader_program();
        vbo = create_vertex_buffer(shader_program, position_buffer);
        // Use shader program and bind vertex buffer for rendering
        while (loop_glfw_window(window))
        {
            glClearColor(0.13f, 0.24f, 0.66f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw the triangle
            glUseProgram(shader_program);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glDrawArrays(GL_TRIANGLES, 0, vertex_count);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glUseProgram(0);

            updated_glfw_render(window);
            update_glfw_window();
        }
        cleanup();
    }
    close_glfw_window(window);
    return 0;
}

// prints the position buffer
/*void print_buffer(GLuint buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    vec3* data = (vec3*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    if (data) {
        for (int i = 0; i < vertex_count; i++) {
            printf("Vertex %d: (%f, %f, %f)\n", i, data[i].x, data[i].y, data[i].z);
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    check_opengl_error("print_buffer");
}*/


    /*glUseProgram(compute_program);
    glDispatchCompute(3, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    // glFinish();
    check_opengl_error("run_compute_shader");*/