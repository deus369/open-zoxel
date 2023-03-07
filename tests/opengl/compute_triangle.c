// cc -std=c99 tests/opengl/compute_triangle.c -o compute_triangle -lGL -lGLEW -lglfw && ./compute_triangle
// sudo apt-get install libglew-dev libglfw3-dev
// sudo pacman -S glew glfw-x11
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
    "#version 330 core\n"
    "layout (location = 0) in vec4 position;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = position;\n"
    "}\n\0";
const char* fragment_shader_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const char* compute_shader_source = "\
#version 430\n\
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
    uint index = gl_GlobalInvocationID.x;\
    vec3z position;\
    if (index == 0) {\
        position = vec3z(-0.5, -0.5, 0.0);\
    } else if (index == 1) {\
        position = vec3z(0.5, -0.5, 0.0);\
    } else if (index == 2) {\
        position = vec3z(0.0, 0.5, 0.0);\
    }\
    positions[index] = position;\
}";

void check_opengl_error() {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("check_opengl_error [%i]\n", (int) err);
    }
}

GLFWwindow* setup_window() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(600, 420, "Compute Triangle Example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    return window;
}

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
    check_opengl_error();
    return shader_program;
}

// position buffer used for vertex positions
GLuint setup_position_buffer() {
    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, position_buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, data_length, NULL, GL_DYNAMIC_DRAW);
    check_opengl_error();
    return position_buffer;
}

GLuint create_vertex_buffer(GLuint position_buffer) {
    GLuint position_attrib = 0; // glGetAttribLocation(material, "positions");
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
    check_opengl_error();
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
    check_opengl_error();
}

void attach_buffer_to_compute_program() {
    glUseProgram(compute_program);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, position_buffer);
    glUseProgram(0);
    check_opengl_error();
}

// Dispatch compute shader to generate vertex positions
void run_compute_shader(GLuint compute_program) {
    glUseProgram(compute_program);
    glDispatchCompute(3, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glFinish();
    check_opengl_error();
}

// prints the position buffer
void print_buffer(GLuint buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    vec3* data = (vec3*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    if (data) {
        for (int i = 0; i < vertex_count; i++) {
            printf("Vertex %d: (%f, %f, %f)\n", i, data[i].x, data[i].y, data[i].z);
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    check_opengl_error();
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
    GLFWwindow* window = setup_window();
    position_buffer = setup_position_buffer();
    setup_compute_buffer(position_buffer);
    attach_buffer_to_compute_program();
    run_compute_shader(compute_program);
    print_buffer(position_buffer);
    vbo = create_vertex_buffer(position_buffer);
    shader_program = create_shader_program();
    // Use shader program and bind vertex buffer for rendering
    glUseProgram(shader_program);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cleanup();
    glfwTerminate();
    return 0;
}


    // Unbind vertex buffer
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // works
    // GLuint vbo = create_vertex_buffer();

    // glDeleteVertexArrays(1, &vao);

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        /*glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);*/

    // Set up vertex data and buffer
    /*float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    GLuint VBO, VAO, computeShader;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set up shader program
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
    glUseProgram(shader_program);*/

    /**/

    // Set up vertex array and buffer for rendering
    /*GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * 16, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_COPY_WRITE_BUFFER, position_buffer);
    glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ARRAY_BUFFER, 0, 0, 24 * 16);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);*/

/*const char* compute_shader_source = "\
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;\
\
layout(std430, binding = 0) buffer PositionBuffer {\
    vec4 positions[];\
};\
\
void main() {\
    uint index = gl_GlobalInvocationID.x;\
    vec3 position = vec3((index / 6) % 2, (index / 3) % 2, index % 2);\
    position = position * 2.0 - 1.0;\
    if ((index / 6) % 2 == 1) position = position.yzx;\
    positions[index] = vec4(position, 1.0);\
}";*/




/*GLuint create_vertex_buffer() {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Set up vertices
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    // Copy vertex data to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set up vertex attributes
    GLuint position_attrib = 0;
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(position_attrib);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Return buffer ID
    return vbo;
}*/