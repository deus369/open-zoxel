void print_opengl() {
    #ifdef zoxel_debug_opengl
        zoxel_log(" > opengl\n");
        zoxel_log("     + version   [%s]\n", glGetString(GL_VERSION));
        zoxel_log("     + glsl      [%s]\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        zoxel_log("     + vendor    [%s]\n", glGetString(GL_VENDOR));
        zoxel_log("     + renderer  [%s]\n", glGetString(GL_RENDERER));
        /*GLint memory_used, memory_total;
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &memory_used);
        glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &memory_total);
        zoxel_log("     + GPU Memory Usage [%d MB / %d MB]\n", memory_used / 1024, memory_total / 1024);
        check_opengl_error("[print_opengl Error]");*/
    #endif
}

void opengl_enable_blend() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void opengl_disable_blend() {
    glDisable(GL_BLEND);
}

void opengl_disable_texture(unsigned char isBlend) {
    if (isBlend) glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void opengl_clear() {
    glClearColor(opengl_clear_color.x, opengl_clear_color.y, opengl_clear_color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // Clears the buffer ?
    glEnable(GL_DEPTH_TEST);        // cull for 3D things
    glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK); // defaults to this
}

uint spawn_gpu_texture_buffers() {
    const int textureType = GL_NEAREST; // GL_LINEAR
    uint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureType);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

//! Spawns the buffers for a mesh indicies and verts on the gpu.
uint2 spawn_gpu_mesh_buffers() {
    uint2 mesh;
    glGenBuffers(1, &mesh.x);
    glGenBuffers(1, &mesh.y);
    #ifdef zoxel_catch_opengl_errors
        check_opengl_error("spawn_gpu_mesh_buffers");
    #endif
    return mesh;
}

uint spawn_gpu_generic_buffer() {
    uint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

unsigned char opengl_set_material(uint material) {
    if (material == 0) return 0;
    glUseProgram(material);
    #ifdef zoxel_catch_opengl_errors
        check_opengl_error("opengl_set_material");
    #endif
    return 1;
}

void opengl_bind_mesh(uint2 mesh) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.x);    // for indices
    glBindBuffer(GL_ARRAY_BUFFER, mesh.y);            // for vertex coordinates
}

void opengl_set_mesh_indicies(uint indices_buffer) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);    // for indices
}

void opengl_set_mesh_uvs(uint uv_buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);         // for UV coordinates
}

void opengl_unset_mesh() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void opengl_render(int indicies_length) {
    // zoxel_log(" > mesh_indicies_length: %i\n", indicies_length);
    glDrawElements(GL_TRIANGLES, indicies_length, GL_UNSIGNED_INT, NULL);
    #ifdef zoxel_catch_opengl_errors
        check_opengl_error("opengl_render");
    #endif
}

void opengl_disable_opengl_program() {
    glUseProgram(0);
}

// glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, (const GLfloat*) ((float*) &view_matrix));

/*
Multiple textures:

    https://learnopengl.com/Getting-started/textures

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

*/

// glActiveTexture(GL_TEXTURE0);
// glUniform1i(texture, 0);
// glEnableTexture(texture);
// glActiveTexture(GL_TEXTURE0);
// glUniform1i(texture, 0);
// Error 0x500/1280 means GL_INVALID_ENUM

void opengl_enable_vertex_buffer(uint shader_index, uint vertex_buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glEnableVertexAttribArray(shader_index);
    glVertexAttribPointer(shader_index, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void opengl_enable_uv_buffer(uint shader_index, uint uv_buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glEnableVertexAttribArray(shader_index);
    glVertexAttribPointer(shader_index, 2, GL_FLOAT, GL_FALSE,  0, 0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void opengl_enable_color_buffer(uint shader_index, uint color_buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glEnableVertexAttribArray(shader_index);
    glVertexAttribPointer(shader_index, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void opengl_disable_buffer(uint shader_index) {
    glDisableVertexAttribArray(shader_index);
}

void opengl_set_matrix(uint shader_index, const float4x4 view_matrix) {
    glUniformMatrix4fv(shader_index, 1, GL_FALSE, (float*) &view_matrix);
}