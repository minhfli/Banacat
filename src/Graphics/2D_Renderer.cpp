#include <iostream>

#include "2D_Renderer.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture2D.h"

#include "Shader.h"

namespace sk_graphic {
    static const size_t maxQuadCounts = 10000;
    static const size_t maxQuadVertexCounts = maxQuadCounts * 4;
    static const size_t maxQuadInDexCounts = maxQuadCounts * 6;

    static const size_t maxLineCounts = 30000;
    static const size_t maxLineVertexCounts = maxLineCounts * 2; //* lines dont need index and texture

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
        int screen_w, screen_h;

        Camera* cam_object;

        GLuint Texture_slot[maxTextureCounts];
        Texture2D default_tex;

        struct quad_data {
            VertexArray vao;
            VertexBuffer vbo;
            IndexBuffer ebo;
            struct vertex {
                glm::vec3 pos;
                glm::vec4 color = glm::vec4(1);
                glm::vec2 uv;     //? texture coord
                GLuint texture;   //? texture index
            };
            vertex* vertices = nullptr;
            Shader shader;
            GLuint vertex_count;
            GLuint index_count;
        }quad;

        struct line_data {
            VertexArray vao;
            VertexBuffer vbo;
            struct vertex {
                glm::vec3 pos;
                glm::vec4 color = glm::vec4(1);
            };
            vertex* vertices = nullptr;
            Shader shader;
            GLuint vertex_count;
        }line;


    };
    static RenderData rdata;

    //! Temporary
    void Renderer2D_GetCam(Camera* cam) {
        rdata.cam_object = cam;
    }

    void Renderer2D_Init() {
        glClearColor(0.1, 0.2, 0.3, 1.0);

        glEnable(GL_DEPTH_TEST);

        //? opengl wont draw back side of trianggle
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        rdata.quad.vertices = new RenderData::quad_data::vertex[maxQuadVertexCounts];

        rdata.quad.vao.Gen(); rdata.quad.vao.Bind();
        rdata.quad.vbo.Gen(); rdata.quad.vbo.Bind();
        rdata.quad.ebo.Gen(); rdata.quad.ebo.Bind();

        //* set up VAO and VBO
        rdata.quad.vbo.Set_Data(nullptr, maxQuadVertexCounts * sizeof(vertex), GL_DYNAMIC_DRAW);

        rdata.quad.vao.Attrib(rdata.quad.vbo, 0, 3, GL_FLOAT, sizeof(vertex), (void*)offsetof(vertex, pos));
        rdata.quad.vao.Attrib(rdata.quad.vbo, 1, 4, GL_FLOAT, sizeof(vertex), (void*)offsetof(vertex, color));
        rdata.quad.vao.Attrib(rdata.quad.vbo, 2, 2, GL_FLOAT, sizeof(vertex), (void*)offsetof(vertex, uv));
        rdata.quad.vao.Attrib(rdata.quad.vbo, 3, 1, GL_UNSIGNED_INT, sizeof(vertex), (void*)offsetof(vertex, texture));

        //* set up EBO
        GLuint offset = 0;
        GLuint indices[maxQuadInDexCounts];
        for (GLuint i = 0; i < maxQuadInDexCounts; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;  //? Quad vertex order ?
            indices[i + 2] = offset + 2;    //*    2--------3
            indices[i + 3] = offset + 2;    //*    |        |
            indices[i + 4] = offset + 1;    //*    0--------1
            indices[i + 5] = offset + 3;
            offset += 4;
        }
        //ebo_Add_Data(rdata.ebo, indices, maxInDexCounts * sizeof(GLuint), GL_DYNAMIC_DRAW);
        rdata.quad.ebo.Set_Data(indices, maxQuadInDexCounts * sizeof(GLuint), GL_DYNAMIC_DRAW);

        //* setup Shader
        rdata.quad.shader.Compile("Assets/Shaders/test.vert", "Assets/Shaders/test.frag");
        rdata.quad.shader.Use();

        //* setup textures
        rdata.default_tex.load("Assets/error.png");
        memset(rdata.Texture_slot, rdata.default_tex.ID, sizeof(rdata.Texture_slot));


        rdata.default_tex.Bind(0);
    }

    void Renderer2D_ShutDown() {
        delete[] rdata.quad.vertices;

        rdata.quad.vao.Delete();
        rdata.quad.vbo.Delete();
        rdata.quad.ebo.Delete();
        rdata.quad.shader.Delete();
    }

    void Renderer2D_Begin() {
        //std::cout << "start new frame\n";
    }

    void Renderer2D_End() {
        if (rdata.quad.index_count) Renderer2D_FlushQuads();
        //std::cout << "frame ended";

    }
    void Renderer2D_AddQuad(glm::vec3 pos, glm::vec2 size, glm::vec4 color) {
        //std::cout << "quad added\n";
        if (rdata.quad.vertex_count >= maxQuadVertexCounts) {
            Renderer2D_FlushQuads();
        }
        glm::vec2 hsize = size / 2.0f; //? half the size

        //* vertex 0
        rdata.quad.vertices[rdata.quad.vertex_count].pos = glm::vec3(pos.x - hsize.x, pos.y - hsize.y, pos.z);
        rdata.quad.vertices[rdata.quad.vertex_count].uv = glm::vec2(0, 1);
        rdata.quad.vertices[rdata.quad.vertex_count].color = color;
        rdata.quad.vertices[rdata.quad.vertex_count].texture = rdata.default_tex.ID;
        rdata.quad.vertex_count++;

        //* vertex 1
        rdata.quad.vertices[rdata.quad.vertex_count].pos = glm::vec3(pos.x + hsize.x, pos.y - hsize.y, pos.z);
        rdata.quad.vertices[rdata.quad.vertex_count].uv = glm::vec2(1, 1);
        rdata.quad.vertices[rdata.quad.vertex_count].color = color;
        rdata.quad.vertices[rdata.quad.vertex_count].texture = rdata.default_tex.ID;
        rdata.quad.vertex_count++;

        //* vertex 2
        rdata.quad.vertices[rdata.quad.vertex_count].pos = glm::vec3(pos.x - hsize.x, pos.y + hsize.y, pos.z);
        rdata.quad.vertices[rdata.quad.vertex_count].uv = glm::vec2(0, 0);
        rdata.quad.vertices[rdata.quad.vertex_count].color = color;
        rdata.quad.vertices[rdata.quad.vertex_count].texture = rdata.default_tex.ID;
        rdata.quad.vertex_count++;

        //* vertex 3
        rdata.quad.vertices[rdata.quad.vertex_count].pos = glm::vec3(pos.x + hsize.x, pos.y + hsize.y, pos.z);
        rdata.quad.vertices[rdata.quad.vertex_count].uv = glm::vec2(1, 0);
        rdata.quad.vertices[rdata.quad.vertex_count].color = color;
        rdata.quad.vertices[rdata.quad.vertex_count].texture = rdata.default_tex.ID;
        rdata.quad.vertex_count++;

        rdata.quad.index_count += 6;
    }
    void Renderer2D_FlushQuads() {
        //std::cout << "flush\n";
        //std::cout << rdata.vertex_count << " " << rdata.index_count << '\n';
        rdata.cam_object->CamMatrix(rdata.quad.shader);

        rdata.quad.shader.Use();
        rdata.quad.vbo.Upd_Data(rdata.quad.vertices, rdata.quad.vertex_count * sizeof(vertex));
        glDrawElements(GL_TRIANGLES, rdata.quad.index_count, GL_UNSIGNED_INT, 0);

        rdata.quad.vertex_count = 0;
        rdata.quad.index_count = 0;
    }
}