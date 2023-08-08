uint2 shader3D_textured;
Material3DTextured attributes_textured3D;

void spawn_attributes_textured3D(uint material) {
    attributes_textured3D = (Material3DTextured) { 
        .vertex_position = glGetAttribLocation(material, "vertex_position"),
        .vertex_uv = glGetAttribLocation(material, "vertex_uv"),
        .vertex_color = glGetAttribLocation(material, "vertex_color"),
        .position = glGetUniformLocation(material, "position"),
        .fog_data = glGetUniformLocation(material, "fog_data"),
        .view_matrix = glGetUniformLocation(material, "view_matrix"),
        .rotation = glGetUniformLocation(material, "rotation"),
        .scale = glGetUniformLocation(material, "scale"),
        .brightness = glGetUniformLocation(material, "brightness")
    };
}

void dispose_shader3D_textured() {
    glDeleteShader(shader3D_textured.x);
    glDeleteShader(shader3D_textured.y);
}

int load_shader3D_textured() {
    shader3D_textured = spawn_gpu_shader_inline(shader3D_textured_vert_buffer, shader3D_textured_frag_buffer);
    #ifdef zoxel_debug_opengl
        zoxel_log(" > loaded shader3D textured\n");
    #endif
    return 0;
}

void opengl_set_texture_only(uint texture_buffer) {
    glBindTexture(GL_TEXTURE_2D, texture_buffer);
}

void opengl_set_texture(uint texture_buffer, unsigned char isBlend) {
    if (isBlend) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    } else {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_BLEND);
    }
    glBindTexture(GL_TEXTURE_2D, texture_buffer);
}

void opengl_set_material3D_uvs_properties(float4 rotation, float scale, float brightness, Material3DTextured *attributes) {
    // glUniform3f(materialTextured3D->position, position.x, position.y, position.z);
    glUniform4f(attributes->rotation, rotation.x, rotation.y, rotation.z, rotation.w);
    glUniform1f(attributes->scale, scale);
    glUniform1f(attributes->brightness, brightness);
}

void opengl_set_material3D_uvs_position(float3 position, Material3DTextured *attributes) {
    glUniform3f(attributes->position, position.x, position.y, position.z);
}

void opengl_shader3D_textured_set_camera_view_matrix(const float4x4 view_matrix, Material3DTextured *attributes) {
    glUniformMatrix4fv(attributes->view_matrix, 1, GL_FALSE, (float*) &view_matrix);
}

void opengl_upload_shader3D_textured(uint2 mesh_buffer, uint uv_buffer, uint color_buffer, const int *indicies, int indicies_length, const float3 *verts, int verts_length, const float2 *uvs, const color_rgb *color_rgbs) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_buffer.x);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies_length * sizeof(int), indicies, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_buffer.y);
    glBufferData(GL_ARRAY_BUFFER, verts_length * sizeof(float3), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, verts_length * sizeof(float2), uvs, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, verts_length * sizeof(color_rgb), color_rgbs, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    #ifdef zoxel_catch_opengl_errors
        check_opengl_error("opengl_upload_shader3D_textured");
    #endif
}

void opengl_set_buffer_attributes(uint vertex_buffer, uint uv_buffer, uint color_buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glEnableVertexAttribArray(attributes_textured3D.vertex_position);
    glVertexAttribPointer(attributes_textured3D.vertex_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glEnableVertexAttribArray(attributes_textured3D.vertex_uv);
    glVertexAttribPointer(attributes_textured3D.vertex_uv, 2, GL_FLOAT, GL_FALSE,  0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glEnableVertexAttribArray(attributes_textured3D.vertex_color);
    glVertexAttribPointer(attributes_textured3D.vertex_color, sizeof(color_rgb), GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
}

void render_textured3D(uint2 mesh_buffer, uint uv_buffer, uint color_buffer, uint mesh_indicies_length, float3 position) {
    opengl_set_mesh_indicies(mesh_buffer.x);
    opengl_set_buffer_attributes(mesh_buffer.y, uv_buffer, color_buffer);
    glUniform3f(attributes_textured3D.position, position.x, position.y, position.z);
    /*glBindBuffer(GL_ARRAY_BUFFER, mesh.y);
    glEnableVertexAttribArray(attributes_textured3D.vertex_position);
    glVertexAttribPointer(attributes_textured3D.vertex_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glEnableVertexAttribArray(attributes_textured3D.vertex_uv);
    glVertexAttribPointer(attributes_textured3D.vertex_uv, 2, GL_FLOAT, GL_FALSE,  0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glEnableVertexAttribArray(attributes_textured3D.vertex_color);
    glVertexAttribPointer(attributes_textured3D.vertex_color, sizeof(color_rgb), GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);*/
    // glUniform4f(materialColored3D.rotation, rotation.x, rotation.y, rotation.z, rotation.w);
    // opengl_set_material3D_uvs_position(position3D->value, &attributes);
    #ifndef zox_disable_render_terrain_chunks
        opengl_draw_triangles(mesh_indicies_length);
    #endif
}