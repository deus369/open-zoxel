//! Functions for handling Textured2DMaterial (SquareTextured2D)
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

//! Keep property reference in material, upon creation.
// #define DEVBUILD
// first, get working on pc
//  then, move texture updating to the entity system
// texture issues
//  https://www.reddit.com/r/opengl/comments/ydsqkn/textured_square_works_on_pinephone_pro_but_not_pc/
//  https://github.com/edo9300/edopro/issues/151
// const char *playerCharacterTextureName = "resources/textures/Test.png";
const bool disableTextureLoaded = false;
int textureType = GL_NEAREST; // GL_LINEAR
//! \todo Move these references to MaterialGPULink
//! \todo Update texture based on Player Entity texture updateing
const char* texturedRender2DVertPath = "resources/shaders/2D/TexturedRender2D.vert";
const char* texturedRender2DFragPath = "resources/shaders/2D/TexturedRender2D.frag";
//! shaders
GLuint texturedVertShader;
GLuint texturedFragShader;
// MaterialGPULink and properties
GLuint texturedMaterial;
//! Mesh B - Buffers/Texture
GLuint squareTexturedModelIndicies;
GLuint squareTexturedModelVertices;
GLuint squareTexturedModelUVs;

void DisposeTexturedMaterial2D()
{
    glDeleteBuffers(1, &squareTexturedModelIndicies);
    glDeleteBuffers(1, &squareTexturedModelVertices);
    glDeleteBuffers(1, &squareTexturedModelUVs);
    // glDeleteTextures(1, &textureID);
    glDeleteShader(texturedVertShader);
    glDeleteShader(texturedFragShader);
    glDeleteProgram(texturedMaterial);
#ifdef DEVBUILD
    GLenum err7 = glGetError();
    if (err7 != GL_NO_ERROR)
    {
        printf("GL HAD ERROR with end of EndAppOpenGL: %i\n", err7);
    }
#endif
}

void InitializeMaterialPropertiesB(GLuint material, MaterialTextured2D *materialTextured2D)
{
    materialTextured2D->view_matrix = glGetUniformLocation(material, "viewMatrix");
    materialTextured2D->positionX = glGetUniformLocation(material, "positionX");
    materialTextured2D->positionY = glGetUniformLocation(material, "positionY");
    materialTextured2D->angle = glGetUniformLocation(material, "angle");
    materialTextured2D->scale = glGetUniformLocation(material, "scale");
    materialTextured2D->brightness = glGetUniformLocation(material, "brightness");
    materialTextured2D->vertexPosition = glGetAttribLocation(material, "vertexPosition");
    materialTextured2D->vertexUV = glGetAttribLocation(material, "vertexUV");
    materialTextured2D->texture = glGetUniformLocation(material, "tex");
}

void InitializeTexturedMesh(GLuint material)
{
    MaterialTextured2D materialTextured2D;
    InitializeMaterialPropertiesB(material, &materialTextured2D);
    // gen buffers
    glGenBuffers(1, &squareTexturedModelIndicies);
    glGenBuffers(1, &squareTexturedModelVertices);  // generate a new VBO and get the associated ID
    glGenBuffers(1, &squareTexturedModelUVs);  // generate a new VBO and get the associated ID
    // indicies
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareTexturedModelIndicies);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareTexturedIndicies2), squareTexturedIndicies2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //verts
    glBindBuffer(GL_ARRAY_BUFFER, squareTexturedModelVertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareTexturedVerts), squareTexturedVerts, GL_STATIC_DRAW); 
    glVertexAttribPointer(materialTextured2D.vertexPosition, 2, GL_FLOAT, GL_FALSE, 16, (GLvoid*)(0 * sizeof(float)));
    glVertexAttribPointer(materialTextured2D.vertexUV, 2, GL_FLOAT, GL_FALSE, 16, (GLvoid*)(2 * sizeof(float)));
    glEnableVertexAttribArray(materialTextured2D.vertexPosition);
    glEnableVertexAttribArray(materialTextured2D.vertexUV);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#ifdef DEVBUILD
    GLenum err7 = glGetError();
    if (err7 != GL_NO_ERROR)
    {
        printf("GL HAD ERROR with end of InitializeTexturedMesh: %i\n", err7);
    }
#endif
}

GLuint SpawnTextureGPU()
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureType);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

void RenderEntityMaterial2D(const float4x4 viewMatrix, GLuint material, GLuint texture,
    float2 position, float angle, float scale, float brightness)
{
    if (material == 0)
    {
        // printf("RenderEntityMaterial2D material is 0.\n");
        return;
    }
    MaterialTextured2D materialTextured2D;
    InitializeMaterialPropertiesB(material, &materialTextured2D);
    glUseProgram(material);   // invalid operation
    /*GLenum err65 = glGetError();
    if (err65 != GL_NO_ERROR)
    {
        printf("GLError [glUseProgram(material) %i]: %i\n", material, err65);
        return;
    }*/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareTexturedModelIndicies);    // for indices
    glBindBuffer(GL_ARRAY_BUFFER, squareTexturedModelVertices);            // for vertex buffer data
    glUniformMatrix4fv(materialTextured2D.view_matrix, 1, GL_FALSE, (const GLfloat*) ((float*) &viewMatrix));
    glUniform1f(materialTextured2D.positionX, position.x);
    glUniform1f(materialTextured2D.positionY, position.y);
    glUniform1f(materialTextured2D.angle, angle);
    glUniform1f(materialTextured2D.scale, scale);
    glUniform1f(materialTextured2D.brightness, brightness);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    /*GLenum err66 = glGetError();
    if (err66 != GL_NO_ERROR)
    {
        printf("GLError [RenderEntityMaterial2D]: %i\n", err66);
        return;
    }*/
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

GLuint CreateTexturedMaterial2D()
{
    GLuint material = glCreateProgram();
    LinkShaderProgram(material, texturedVertShader, texturedFragShader);
    return material;
}

int LoadTextureRender2DShader()
{
    if (LoadShader(texturedRender2DVertPath, GL_VERTEX_SHADER, &texturedVertShader) != 0)
    {
        printf("Error loading shader vert 2D.\n");
        return -1;
    }
    if (LoadShader(texturedRender2DFragPath, GL_FRAGMENT_SHADER, &texturedFragShader) != 0)
    {
        printf("Error loading shader frag 2D.\n");
        return -1;
    }
    texturedMaterial = CreateTexturedMaterial2D();
    InitializeTexturedMesh(texturedMaterial);
    return 0;
}

// #ifdef DEVBUILD
//     GLenum err66 = glGetError();
//     if (err66 != GL_NO_ERROR)
//     {
//         printf("GL HAD ERROR with End of RenderEntityMaterial2D: %i\n", err66);
//         return;
//     }
// #endif