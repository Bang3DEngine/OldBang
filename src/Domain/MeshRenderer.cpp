#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() : mesh(nullptr), material(nullptr)
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

void MeshRenderer::SetMaterial(const Material *m)
{
    material = m;
}

void MeshRenderer::Render(MeshRenderer::DrawingMode drawingMode) const
{
    if(mesh == nullptr)
    {
        Logger::Error("This MeshRenderer doesn't have a Mesh. Can't render.");
        return;
    }

    if(material == nullptr)
    {
        Logger::Error("This MeshRenderer doesn't have a Material. Can't render.");
        return;
    }
    else
    {
        if(material->GetShaderProgram() == nullptr)
        {
            Logger::Error("This MeshRenderer has a Material with a null \
                          ShaderProgram. Can't render.");
            return;
        }
    }


    vao->Bind();
    material->GetShaderProgram()->Bind();

    glDrawArrays(drawingMode, 0, mesh->GetVertexCount());

    material->GetShaderProgram()->UnBind();
    vao->UnBind();
}
