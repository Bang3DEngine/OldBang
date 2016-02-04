#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() : mesh(nullptr)
{
    vao = new VAO();
}

MeshRenderer::~MeshRenderer()
{
    if(vao != nullptr) delete vao;
}

void MeshRenderer::SetMesh(const Mesh *m)
{
    if(m != nullptr)
    {
        meshVerticesPosVBOIndex = vao->BindVBO(m->verticesPosVBO, 3);
    }
    else if(meshVerticesPosVBOIndex != -1)
    {
        vao->UnBindVBO(meshVerticesPosVBOIndex);
        meshVerticesPosVBOIndex = -1;
    }

    mesh = m;
}

void MeshRenderer::Render(const ShaderProgram *shaderProgram,
                          MeshRenderer::DrawingMode drawingMode) const
{
    if(mesh != nullptr)
    {
        vao->Bind();
        shaderProgram->Bind();

        glDrawArrays(drawingMode, 0, mesh->GetVertexCount());

        shaderProgram->UnBind();
        vao->UnBind();
    }
}
