#include "MeshRenderer.h"
#include "Entity.h"

void MeshRenderer::_OnDrawing()
{
    if(GetOwner() != nullptr)
    {
        Render(GetOwner()->GetPart<Transform>(),
               MeshRenderer::DrawingMode::Triangles);
    }
}

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
        vao->BindVBO(m->vertexPositionsVBO, VAO::VBOMeaning::Position);
        vao->BindVBO(m->vertexNormalsVBO,   VAO::VBOMeaning::Normal);
    }
    else
    {
        vao->UnBindVBO(VAO::VBOMeaning::Position);
        vao->UnBindVBO(VAO::VBOMeaning::Normal);
    }

    mesh = m;
}

void MeshRenderer::SetMaterial(const Material *m)
{
    material = m;
}

void MeshRenderer::Render(const Transform *t,
                          MeshRenderer::DrawingMode drawingMode) const
{
    if(t == nullptr)
    {
        Logger::Error("This Entity doesn't have a Transform. Can't render.");
        return;
    }

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
    material->shaderProgram->Bind();

    glm::mat4 matTransform; t->GetMatrix(matTransform);
    material->shaderProgram->SetUniformMat4(ShaderContract::Uniform_Matrix_Model,
                                            matTransform);


    glDrawArrays(drawingMode, 0, mesh->GetVertexCount());

    material->shaderProgram->UnBind();
    vao->UnBind();
}
