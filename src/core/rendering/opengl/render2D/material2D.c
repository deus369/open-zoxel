//! Links to locations inside a base material->
typedef struct {
    GLint vertex_position;
    uint view_matrix;
    uint positionX;
    uint positionY;
    uint angle;
    uint scale;
    uint brightness;
} Material2D;

void initialize_material2D_properties(Material2D *material2D, uint material) {
    material2D->view_matrix = glGetUniformLocation(material, "camera_matrix");
    material2D->angle = glGetUniformLocation(material, "angle");
    material2D->scale = glGetUniformLocation(material, "scale");
    material2D->brightness = glGetUniformLocation(material, "brightness");
    material2D->positionX = glGetUniformLocation(material, "positionX");
    material2D->positionY = glGetUniformLocation(material, "positionY");
    material2D->vertex_position = glGetAttribLocation(material, "vertex_position");
}