#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() : vao(nullptr)
{
}

MeshRenderer::~MeshRenderer()
{
    if(vao != nullptr) delete vao;
}

void MeshRenderer::Render(const ShaderProgram *shaderProgram, const Mesh *mesh,
                          MeshRenderer::DrawingMode drawingMode) const
{
    vao->Bind();
    shaderProgram->Bind();

    glDrawArrays(drawingMode, 0, mesh->GetVertexCount());

    shaderProgram->UnBind();
    vao->UnBind();
}
