void Line3DRenderSystem(ecs_iter_t *it) {
    glUseProgram(line3D_material);
    glEnableVertexAttribArray(line3D_position_location);
    opengl_set_float4(line3D_fog_data_location, (float4) { fog_color.x, fog_color.y, fog_color.z, get_fog_density() });
    opengl_enable_blend();
    glUniformMatrix4fv(line3D_camera_matrix_location, 1, GL_FALSE, (GLfloat*) &render_camera_matrix);
    zox_field_in(LineData3D, lineData3Ds, 1)
    zox_field_in(LineThickness, lineThicknesss, 2)
    zox_field_in(ColorRGB, colorRGBs, 3)
    for (int i = 0; i < it->count; i++) {
        zox_field_i_in(LineData3D, lineData3Ds, lineData3D)
        zox_field_i_in(LineThickness, lineThicknesss, lineThickness)
        zox_field_i_in(ColorRGB, colorRGBs, colorRGB)
        const float3 color_rgb_f3 = color_rgb_to_float3(colorRGB->value);
        glLineWidth(lineThickness->value);
        glVertexAttribPointer(line3D_position_location, 3, GL_FLOAT, GL_FALSE, 0, (GLfloat*) &lineData3D->value);
        glUniform3f(line3D_color_location, color_rgb_f3.x, color_rgb_f3.y, color_rgb_f3.z);
        glDrawArrays(GL_LINES, 0, 2);
    }
    glDisableVertexAttribArray(line3D_position_location);
    opengl_disable_blend();
    glUseProgram(0);
} zox_declare_system(Line3DRenderSystem)
