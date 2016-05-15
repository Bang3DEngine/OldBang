#include "Renderer.h"

#include "GameObject.h"
#include "Camera.h"
#include "Scene.h"
#include "Transform.h"

Renderer::Renderer()
{
}

Material *Renderer::GetMaterial()
{
    return material;
}

void Renderer::ActivateStatesBeforeRendering() const
{
    glLineWidth(lineWidth);
    glEnable(GL_LINE_SMOOTH); //Line antialiasing

    //Set polygon mode
    if(drawWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //Set culling mode
    if(cullMode != CullMode::None)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(cullMode);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

void Renderer::GetMatrices(glm::mat4 &model,
                           glm::mat4 &view,
                           glm::mat4 &projection,
                           glm::mat4 &pvm) const
{
    //We assume cam, scene and transform do exist.

    Camera *cam = owner->GetScene()->GetCamera();

    model = glm::mat4(1.0f);
    if(!ignoreModelMatrix)
    {
        Transform *t = owner->GetComponent<Transform>();
        if(t != nullptr) t->GetMatrix(model);
    }

    view = glm::mat4(1.0f);
    if(!ignoreViewMatrix)
    {
        cam->GetViewMatrix(view);
    }

    projection = glm::mat4(1.0f);
    cam->GetProjectionMatrix(projection);

    pvm = projection * view * model;
}

void Renderer::SetMatricesUniforms(const glm::mat4 &model,
                                   const glm::mat4 &view,
                                   const glm::mat4 &projection,
                                   const glm::mat4 &pvm) const
{
    material->shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Model, model, false);
    material->shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_View, view, false);
    material->shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Projection, projection, false);
    material->shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_PVM, pvm, false);
}

void Renderer::SetDrawWireframe(bool drawWireframe)
{
    this->drawWireframe = drawWireframe;
}
bool Renderer::GetDrawWireframe() const
{
    return drawWireframe;
}


void Renderer::SetCullMode(Renderer::CullMode cullMode)
{
    this->cullMode = cullMode;
}
Renderer::CullMode Renderer::GetCullMode() const
{
    return cullMode;
}


void Renderer::SetRenderMode(Renderer::RenderMode renderMode)
{
    this->renderMode = renderMode;
}
Renderer::RenderMode Renderer::GetRenderMode() const
{
    return renderMode;
}


void Renderer::SetLineWidth(float w)
{
    lineWidth = w;
}
float Renderer::GetLineWidth() const
{
    return lineWidth;
}

void Renderer::SetIgnoreModelMatrix(bool ignore)
{
    this->ignoreModelMatrix = ignore;
}

bool Renderer::GetIgnoreModelMatrix() const
{
    return ignoreModelMatrix;
}

void Renderer::SetIgnoreViewMatrix(bool ignore)
{
    ignoreViewMatrix = ignore;
}

bool Renderer::GetIgnoreViewMatrix() const
{
    return ignoreViewMatrix;
}
