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

/*
    ?Quad vertex order
    *   2       3
    *
    *   0       1

    ?Texture uv oder (xy)
    *   00      10
    *
    *   01      11
*/
struct RenderData {
    VertexArray quad_vao;
    VertexBuffer quad_vbo;
    IndexBuffer quad_ebo;

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

    rdata.quad_vao.Gen(); rdata.quad_vao.Bind();
    rdata.quad_vbo.Gen(); rdata.quad_vbo.Bind();
    rdata.quad_ebo.Gen(); rdata.quad_ebo.Bind();

    //* set up VAO and VBO
    rdata.quad_vbo.Set_Data(nullptr, maxVertexCounts * sizeof(vertex), GL_DYNAMIC_DRAW);

    rdata.quad_vao.Attrib(rdata.quad_vbo, 0, 3, GL_FLOAT, sizeof(vertex), (void*)offsetof(vertex, pos));
    rdata.quad_vao.Attrib(rdata.quad_vbo, 1, 4, GL_FLOAT, sizeof(vertex), (void*)offsetof(vertex, color));
    rdata.quad_vao.Attrib(rdata.quad_vbo, 2, 2, GL_FLOAT, sizeof(vertex), (void*)offsetof(vertex, uv));
    rdata.quad_vao.Attrib(rdata.quad_vbo, 3, 1, GL_UNSIGNED_INT, sizeof(vertex), (void*)offsetof(vertex, texture));

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
    //ebo_Add_Data(rdata.ebo, indices, maxInDexCounts * sizeof(GLuint), GL_DYNAMIC_DRAW);
    rdata.quad_ebo.Set_Data(indices, maxInDexCounts * sizeof(GLuint), GL_DYNAMIC_DRAW);

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

    rdata.quad_vao.Delete();
    rdata.quad_vbo.Delete();
    rdata.quad_ebo.Delete();
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
    rdata.vertices[rdata.vertex_count].uv = glm::vec2(0, 1);
    rdata.vertices[rdata.vertex_count].color = color;
    rdata.vertices[rdata.vertex_count].texture = rdata.default_tex.ID;
    rdata.vertex_count++;

    //* vertex 1
    rdata.vertices[rdata.vertex_count].pos = glm::vec3(pos.x + hsize.x, pos.y - hsize.y, pos.z);
    rdata.vertices[rdata.vertex_count].uv = glm::vec2(1, 1);
    rdata.vertices[rdata.vertex_count].color = color;
    rdata.vertices[rdata.vertex_count].texture = rdata.default_tex.ID;
    rdata.vertex_count++;

    //* vertex 2 
    rdata.vertices[rdata.vertex_count].pos = glm::vec3(pos.x - hsize.x, pos.y + hsize.y, pos.z);
    rdata.vertices[rdata.vertex_count].uv = glm::vec2(0, 0);
    rdata.vertices[rdata.vertex_count].color = color;
    rdata.vertices[rdata.vertex_count].texture = rdata.default_tex.ID;
    rdata.vertex_count++;

    //* vertex 3 
    rdata.vertices[rdata.vertex_count].pos = glm::vec3(pos.x + hsize.x, pos.y + hsize.y, pos.z);
    rdata.vertices[rdata.vertex_count].uv = glm::vec2(1, 0);
    rdata.vertices[rdata.vertex_count].color = color;
    rdata.vertices[rdata.vertex_count].texture = rdata.default_tex.ID;
    rdata.vertex_count++;

    rdata.index_count += 6;
}
void Renderer2D_FlushBatch() {
    //std::cout << "flush\n";
    //std::cout << rdata.vertex_count << " " << rdata.index_count << '\n';

    rdata.shader.Use();
    rdata.quad_vbo.Upd_Data(rdata.vertices, rdata.vertex_count * sizeof(vertex));
    glDrawElements(GL_TRIANGLES, rdata.index_count, GL_UNSIGNED_INT, 0);

    rdata.vertex_count = 0;
    rdata.index_count = 0;
}
