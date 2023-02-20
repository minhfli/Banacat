#include <iostream>

#include "2D_Renderer.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture2D.h"

#include "Shader.h"

#include "Vertex2D.h"

static const size_t maxQuadCounts = 10000;
static const size_t maxVertexCounts = maxQuadCounts * 4;
static const size_t maxInDexCounts = maxQuadCounts * 6;
static const size_t maxTextureCounts = 32; //! This can change on different computer

struct RenderData {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    vertex* vertices = nullptr;

    GLuint Texture_slot[maxTextureCounts];
    Texture2D default_tex;

    Shader shader;


    GLuint vertex_count;
    GLuint index_count;
};
static RenderData rdata;

//! Temporary
GLuint GetShaderID() { return rdata.shader.ID; }

void Renderer2D_Init() {
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
    for (GLuint i = 0; i < maxInDexCounts; i += 6) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;  //? Quad vertex order ?
        indices[i + 2] = offset + 2;    //*    2--------3 
        indices[i + 3] = offset + 2;    //*    |        | 
        indices[i + 4] = offset + 1;    //*    0--------1 
        indices[i + 5] = offset + 3;
        offset += 4;
    }
    ebo_Add_Data(rdata.ebo, indices, maxInDexCounts * sizeof(GLuint), GL_DYNAMIC_DRAW);

    //* setup Shader
    rdata.shader.Compile("Assets/Shaders/test.vert", "Assets/Shaders/test.frag");
    rdata.shader.Use();

    //* setup textures
    rdata.default_tex.load("Assets/error.png");
    memset(rdata.Texture_slot, rdata.default_tex.ID, sizeof(rdata.Texture_slot));


    rdata.default_tex.Bind(0);
}

void Renderer2D_ShutDown() {
    delete[] rdata.vertices;

    vao_Delete(rdata.vao);
    vbo_Delete(rdata.vbo);
    ebo_Delete(rdata.ebo);

    rdata.shader.Delete();
}

void Renderer2D_BeginBatch() {
    //std::cout << "start new batch\n";
}

void Renderer2D_EndBatch() {
    if (rdata.index_count) Renderer2D_FlushBatch();
    //std::cout << "batch ended";

}
void Renderer2D_AddQuad(glm::vec3 pos, glm::vec2 size, glm::vec4 color) {
    //std::cout << "quad added\n";
    if (rdata.vertex_count >= maxVertexCounts) {
        Renderer2D_FlushBatch();
        Renderer2D_BeginBatch();
    }
    glm::vec2 hsize = size / 2.0f; //? half the size

    //* vertex 0 
    rdata.vertices[rdata.vertex_count].pos = glm::vec3(pos.x - hsize.x, pos.y - hsize.y, pos.z);
    rdata.vertices[rdata.vertex_count].uv = glm::vec2(0, 0);
    rdata.vertices[rdata.vertex_count].color = color;
    rdata.vertices[rdata.vertex_count].texture = rdata.default_tex.ID;
    rdata.vertex_count++;

    //* vertex 1
    rdata.vertices[rdata.vertex_count].pos = glm::vec3(pos.x + hsize.x, pos.y - hsize.y, pos.z);
    rdata.vertices[rdata.vertex_count].uv = glm::vec2(1, 0);
    rdata.vertices[rdata.vertex_count].color = color;
    rdata.vertices[rdata.vertex_count].texture = rdata.default_tex.ID;
    rdata.vertex_count++;

    //* vertex 2 
    rdata.vertices[rdata.vertex_count].pos = glm::vec3(pos.x - hsize.x, pos.y + hsize.y, pos.z);
    rdata.vertices[rdata.vertex_count].uv = glm::vec2(0, 1);
    rdata.vertices[rdata.vertex_count].color = color;
    rdata.vertices[rdata.vertex_count].texture = rdata.default_tex.ID;
    rdata.vertex_count++;

    //* vertex 3 
    rdata.vertices[rdata.vertex_count].pos = glm::vec3(pos.x + hsize.x, pos.y + hsize.y, pos.z);
    rdata.vertices[rdata.vertex_count].uv = glm::vec2(1, 1);
    rdata.vertices[rdata.vertex_count].color = color;
    rdata.vertices[rdata.vertex_count].texture = rdata.default_tex.ID;
    rdata.vertex_count++;

    rdata.index_count += 6;
}
void Renderer2D_FlushBatch() {
    //std::cout << "flush\n";
    //std::cout << rdata.vertex_count << " " << rdata.index_count << '\n';

    rdata.shader.Use();
    vbo_Update_Data(rdata.vbo, rdata.vertices, rdata.vertex_count * sizeof(vertex));
    glDrawElements(GL_TRIANGLES, rdata.index_count, GL_UNSIGNED_INT, 0);

    rdata.vertex_count = 0;
    rdata.index_count = 0;
}
