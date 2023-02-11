
#include "2D_Renderer.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "Vertex2D.h"

static const size_t maxQuadCounts = 10000;
static const size_t maxVertexCounts = maxQuadCounts * 4;
static const size_t maxInDexCounts = maxQuadCounts * 6;
static const size_t maxTextureCounts = 31; //! This can change on different computer

struct RenderData {
    VAO Quad_VAO;
    VBO Quad_VBO;
    EBO Quad_EBO;

    vertex* vertices = nullptr;

};

RenderData& render_data = *(new RenderData);

void Renderer2D::Init() {
    //render_data = *(new RenderData);
    //render_data.vertices = new vertex[maxVertexCounts];
}

void Renderer2D::ShutDown() {

}

void Renderer2D::BeginBatch() {

}

void Renderer2D::EndBatch() {

}

void Renderer2D::Flush() {

}
