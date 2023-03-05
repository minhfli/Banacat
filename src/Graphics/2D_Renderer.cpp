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
    static float line_width = 0.1f; //* line width is in normalized screeen coordinate not world corrdinate

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
            GLSLprogram shader;
            GLuint vertex_count = 0;
            GLuint index_count = 0;
        }quad;

        struct line_data {
            VertexArray vao;
            VertexBuffer vbo;
            struct vertex {
                glm::vec3 pos;
                glm::vec4 color = glm::vec4(1);
            };
            vertex* vertices = nullptr;
            GLSLprogram shader;
            GLuint vertex_count = 0;
        }line;


    };
    static RenderData rdata;

    typedef RenderData::quad_data::vertex quad_vertex;
    typedef RenderData::line_data::vertex line_vertex;

    //! Temporary
    void Renderer2D_GetCam(Camera* cam) {
        rdata.cam_object = cam;
    }

    void Setup_quad_batch() {
        VertexArray& vao = rdata.quad.vao;
        VertexBuffer& vbo = rdata.quad.vbo;
        IndexBuffer& ebo = rdata.quad.ebo;

        rdata.quad.vertices = new quad_vertex[maxQuadVertexCounts];

        vao.Gen(); vao.Bind();
        vbo.Gen(); vbo.Bind();
        ebo.Gen(); ebo.Bind();

        //* set up VAO and VBO
        GLuint vertex_size = sizeof(quad_vertex);
        rdata.quad.vbo.Set_Data(nullptr, maxQuadVertexCounts * vertex_size, GL_DYNAMIC_DRAW);

        vao.Attrib(vbo, 0, 3, GL_FLOAT, vertex_size, (void*)offsetof(quad_vertex, pos));
        vao.Attrib(vbo, 1, 4, GL_FLOAT, vertex_size, (void*)offsetof(quad_vertex, color));
        vao.Attrib(vbo, 2, 2, GL_FLOAT, vertex_size, (void*)offsetof(quad_vertex, uv));
        vao.Attrib(vbo, 3, 1, GL_UNSIGNED_INT, vertex_size, (void*)offsetof(quad_vertex, texture));

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
        ebo.Set_Data(indices, maxQuadInDexCounts * sizeof(GLuint), GL_STATIC_DRAW);
        // indices[] array is created on stack no need to delete 

        //* setup Shader
        GLSLprogram& shader = rdata.quad.shader;
        shader.Gen();
        shader.Compile("Assets/Shaders/test.vert", GL_VERTEX_SHADER);
        shader.Compile("Assets/Shaders/test.frag", GL_FRAGMENT_SHADER);
        shader.Link();
        shader.Use();
    }
    void Setup_line_batch() {
        VertexArray& vao = rdata.line.vao;
        VertexBuffer& vbo = rdata.line.vbo;

        rdata.line.vertices = new line_vertex[maxLineVertexCounts];

        vao.Gen(); vao.Bind();
        vbo.Gen(); vbo.Bind();

        GLuint vertex_size = sizeof(line_vertex);
        rdata.line.vbo.Set_Data(nullptr, maxLineVertexCounts * vertex_size, GL_DYNAMIC_DRAW);

        rdata.line.vao.Attrib(rdata.line.vbo, 0, 3, GL_FLOAT, vertex_size, (void*)offsetof(line_vertex, pos));
        rdata.line.vao.Attrib(rdata.line.vbo, 1, 4, GL_FLOAT, vertex_size, (void*)offsetof(line_vertex, color));

        //* setup Shader
        GLSLprogram& shader = rdata.line.shader;
        shader.Gen();
        shader.Compile("Assets/Shaders/line.vert", GL_VERTEX_SHADER);
        shader.Compile("Assets/Shaders/line.frag", GL_FRAGMENT_SHADER);
        shader.Compile("Assets/Shaders/line.geom", GL_GEOMETRY_SHADER);
        shader.Link();
        shader.Use();


    }

    void Renderer2D_Init() {
        glClearColor(0.1, 0.2, 0.3, 1.0);

        glEnable(GL_DEPTH_TEST);

        //? opengl wont draw back side of trianggle
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        Setup_quad_batch();
        Setup_line_batch();

        //* setup textures
        rdata.default_tex.load("Assets/error.png");
        memset(rdata.Texture_slot, rdata.default_tex.ID, sizeof(rdata.Texture_slot));

        rdata.default_tex.Bind(0);
    }

    void Renderer2D_ShutDown() {
        delete[] rdata.quad.vertices;
        delete[] rdata.line.vertices;

        rdata.quad.vao.Delete();
        rdata.quad.vbo.Delete();
        rdata.quad.ebo.Delete();
        rdata.quad.shader.Delete();

        rdata.line.vao.Delete();
        rdata.line.vbo.Delete();
        rdata.line.shader.Delete();
    }

    void Renderer2D_Begin() {
        //std::cout << "start new frame\n";
    }
    void Renderer2D_End() {
        if (rdata.quad.index_count) Renderer2D_FlushQuads();
        if (rdata.line.vertex_count) Renderer2D_FlushLines();
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
        rdata.quad.vao.Bind();
        rdata.quad.vbo.Bind();
        rdata.quad.ebo.Bind();
        rdata.quad.vbo.Upd_Data(rdata.quad.vertices, rdata.quad.vertex_count * sizeof(quad_vertex));

        glDrawElements(GL_TRIANGLES, rdata.quad.index_count, GL_UNSIGNED_INT, 0);

        rdata.quad.vertex_count = 0;
        rdata.quad.index_count = 0;
    }

    void Renderer2D_AddLine(glm::vec3 pos, glm::vec2 dir, glm::vec4 color) {
        if (rdata.line.vertex_count >= maxLineVertexCounts)
            Renderer2D_FlushLines();

        rdata.line.vertices[rdata.line.vertex_count].pos = pos;
        rdata.line.vertices[rdata.line.vertex_count].color = color;
        rdata.line.vertex_count++;

        rdata.line.vertices[rdata.line.vertex_count].pos = pos + glm::vec3(dir, 0);
        rdata.line.vertices[rdata.line.vertex_count].color = color;
        rdata.line.vertex_count++;
    }
    void Renderer2D_FlushLines() {
        //std::cout << rdata.line.vertex_count << '\n';

        rdata.cam_object->CamMatrix(rdata.line.shader);
        rdata.line.shader.Use();
        rdata.line.shader.SetFloat("u_screen_aspect", rdata.cam_object->aspect);

        rdata.line.vao.Bind();
        rdata.line.vbo.Bind();
        rdata.line.vbo.Upd_Data(rdata.line.vertices, rdata.line.vertex_count * sizeof(line_vertex));

        glDrawArrays(GL_LINES, 0, rdata.line.vertex_count);

        rdata.line.vertex_count = 0;
    }

    void Renderer2D_AddDotX(glm::vec3 pos, glm::vec4 color) {
        Renderer2D_AddLine(pos + glm::vec3(-0.2f, -0.2f, 1.0f), glm::vec2(0.4f), color);
        Renderer2D_AddLine(pos + glm::vec3(-0.2f, 0.2f, 1.0f), glm::vec2(0.4f, -0.4f), color);
    }
    void Renderer2D_AddDotO(glm::vec3 pos, glm::vec4 color) {
        Renderer2D_AddQuad(pos, glm::vec2(0.4), color);
    }
}