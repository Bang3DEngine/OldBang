#include "Renderer.h"

#include "GameObject.h"
#include "Camera.h"
#include "Scene.h"
#include "Transform.h"

Renderer::Renderer()
{
}

void Renderer::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Renderer *r = static_cast<Renderer*>(clone);
    Component::CloneInto(r);
    r->SetMaterial(material);
    r->SetDrawWireframe(drawWireframe);
    r->SetCullMode(cullMode);
    r->SetRenderMode(renderMode);
    r->SetLineWidth(lineWidth);
    r->SetIgnoreModelMatrix(ignoreModelMatrix);
    r->SetIgnoreViewMatrix(ignoreViewMatrix);
    r->SetIgnoreProjectionMatrix(ignoreProjectionMatrix);
}

Material *Renderer::GetMaterial()
{
    return material;
}

void Renderer::ActivateGLStatesBeforeRendering() const
{
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

    Scene *scene = Canvas::GetCurrentScene();
    Camera *camera = scene->GetCamera();
    if(camera  && material  && material->shaderProgram )
    {
        Transform *t = camera->gameObject->GetComponent<Transform>();
        if(t )
        {
            material->shaderProgram->SetUniformVec3(ShaderContract::Uniform_Position_Camera,
                                                    t->GetPosition(), false);
        }
    }

    glLineWidth(lineWidth);

}

void Renderer::OnRender()
{
    Render();
}

void Renderer::Render() const
{
    NONULL(material); NONULL(material->GetShaderProgram());

    ActivateGLStatesBeforeRendering();
    material->Bind();
    RenderWithoutBindingMaterial();
    material->UnBind();
}

void Renderer::GetMatrices(Matrix4 &model,
                           Matrix4 &view,
                           Matrix4 &projection,
                           Matrix4 &pvm) const
{
    //We assume cam, scene and transform do exist.

    Camera *cam = gameObject->GetScene()->GetCamera();

    if(!ignoreModelMatrix)
    {
        Transform *t = gameObject->GetComponent<Transform>();
        if(t ) t->GetModelMatrix(model);
    }
    else model = Matrix4(1.0f);

    if(!ignoreViewMatrix)
    {
        cam->GetViewMatrix(view);
    }
    else view = Matrix4(1.0f);

    if(!ignoreProjectionMatrix)
    {
        cam->GetProjectionMatrix(projection);
    }
    else projection = Matrix4(1.0f);

    pvm = projection * view * model;
}

void Renderer::SetMatricesUniforms(const Matrix4 &model,
                                   const Matrix4 &view,
                                   const Matrix4 &projection,
                                   const Matrix4 &pvm) const
{
    material->shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Model, model, false);
    material->shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Model_Inverse, model.Inversed(), false);
    material->shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_View, view, false);
    material->shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_View_Inverse, view.Inversed(), false);
    material->shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Projection, projection, false);
    material->shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Projection_Inverse, projection.Inversed(), false);
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

void Renderer::SetIgnoreProjectionMatrix(bool ignore)
{
    ignoreProjectionMatrix = ignore;
}

bool Renderer::GetIgnoreProjectionMatrix() const
{
    return ignoreProjectionMatrix;
}

void Renderer::SetActivateGLStatesBeforeRenderingForSelectionFunction(const std::function<void()> &f)
{
    ActivateGLStatesBeforeRenderingForSelection = f;
}
