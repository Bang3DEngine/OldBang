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

MeshRenderer::MeshRenderer() : mesh(nullptr), material(nullptr), meshVertexPositionsVBOIndex(-1), meshVertexNormalsVBOIndex(-1)
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
        meshVertexPositionsVBOIndex = vao->BindVBO(m->vertexPositionsVBO, 3);
        meshVertexNormalsVBOIndex   = vao->BindVBO(m->vertexNormalsVBO,   3);
    }
    else if(meshVertexPositionsVBOIndex != -1)
    {
        vao->UnBindVBO(meshVertexPositionsVBOIndex);
        meshVertexPositionsVBOIndex = -1;

        vao->UnBindVBO(meshVertexNormalsVBOIndex);
        meshVertexNormalsVBOIndex  = -1;
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
