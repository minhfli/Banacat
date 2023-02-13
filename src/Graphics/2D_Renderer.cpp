
#include "2D_Renderer.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "Shader.h"

#include "Vertex2D.h"

static const size_t maxQuadCounts = 10000;
static const size_t maxVertexCounts = maxQuadCounts * 4;
static const size_t maxInDexCounts = maxQuadCounts * 6;
static const size_t maxTextureCounts = 31; //! This can change on different computer

struct RenderData {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    vertex* vertices = nullptr;

    GLuint Texture_slot[maxTextureCounts];

    Shader shader;
};

static RenderData rdata;

void Renderer2D::Init() {
    rdata.vertices = new vertex[maxVertexCounts];

    rdata.vao = CreateVAO();    vao_Bind(rdata.vao);
    rdata.vbo = CreateVBO();    vbo_Bind(rdata.vbo);
    rdata.ebo = CreateEBO();    ebo_Bind(rdata.ebo);

    //* set up VAO and VBO
    vbo_Add_Data(rdata.vbo, nullptr, maxVertexCounts * sizeof(vertex), GL_DYNAMIC_DRAW);

    vao_link_attrib(rdata.vbo, 0, 3, GL_FLOAT, sizeof(vertex), (void*)offsetof(vertex, pos));
    vao_link_attrib(rdata.vbo, 1, 4, GL_FLOAT, sizeof(vertex), (void*)offsetof(vertex, color));
    vao_link_attrib(rdata.vbo, 2, 2, GL_FLOAT, sizeof(vertex), (void*)offsetof(vertex, uv));
    vao_link_attrib(rdata.vbo, 3, 1, GL_UNSIGNED_INT, sizeof(vertex), (void*)offsetof(vertex, texture));

    //* set up EBO
    GLuint offset = 0;
    GLuint indices[maxInDexCounts];
    for (int i = 0; i < maxInDexCounts; i += 6) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;  //? Quad vertex order ?
        indices[i + 2] = offset + 2;    //*    2--------3 
        indices[i + 3] = offset + 2;    //*    |        | 
        indices[i + 4] = offset + 1;    //*    0--------1 
        indices[i + 5] = offset + 3;
        offset += 4;
    }
    ebo_Add_Data(rdata.ebo, indices, maxInDexCounts * sizeof(GLuint), GL_STATIC_DRAW);

    //* setup Shader
    rdata.shader.Compile("Assets/Shaders/test.vert", "Assets/Shaders/test.frag");
    rdata.shader.Use();
}

void Renderer2D::ShutDown() {
    delete[] rdata.vertices;

    vao_Delete(rdata.vao);
    vbo_Delete(rdata.vbo);
    ebo_Delete(rdata.ebo);

    rdata.shader.Delete();
}

void Renderer2D::BeginBatch() {

}

void Renderer2D::EndBatch() {

}

void Renderer2D::Flush() {

}
