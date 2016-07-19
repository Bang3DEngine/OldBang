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
    r->SetMaterial(p_material);
    r->SetDrawWireframe(m_drawWireframe);
    r->SetCullMode(m_cullMode);
    r->SetRenderMode(m_renderMode);
    r->SetLineWidth(m_lineWidth);
    r->SetIgnoreModelMatrix(m_ignoreModelMatrix);
    r->SetIgnoreViewMatrix(m_ignoreViewMatrix);
    r->SetIgnoreProjectionMatrix(m_ignoreProjectionMatrix);
}

Material *Renderer::GetMaterial()
{
    return p_material;
}

void Renderer::ActivateGLStatesBeforeRendering() const
{
    //Set polygon mode
    if(m_drawWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //Set culling mode
    if(m_cullMode != CullMode::None)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(m_cullMode);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    Scene *scene = Canvas::GetCurrentScene();
    Camera *camera = scene->GetCamera();
    if(camera  && p_material  && p_material->p_shaderProgram )
    {
        Transform *t = camera->gameObject->transform;
        p_material->p_shaderProgram->SetUniformVec3(ShaderContract::Uniform_Position_Camera,
                                                    t->GetPosition(), false);

        p_material->p_shaderProgram->SetUniformFloat("BANG_renderer_receivesLighting",
                                                      m_receivesLighting ? 1.0f : 0.0f, false);
    }

    glLineWidth(m_lineWidth);
}

void Renderer::OnRender()
{
    Render();
}

void Renderer::Render() const
{
    NONULL(p_material); NONULL(p_material->GetShaderProgram());

    ActivateGLStatesBeforeRendering();
    p_material->Bind();
    RenderWithoutBindingMaterial();
    p_material->UnBind();
}

void Renderer::GetMatrices(Matrix4 &model,
                           Matrix4 &view,
                           Matrix4 &projection,
                           Matrix4 &pvm) const
{
    //We assume cam, scene and transform do exist.

    Camera *cam = gameObject->GetScene()->GetCamera();

    if(!m_ignoreModelMatrix)
    {
        Transform *t = gameObject->transform;
        if(t) t->GetModelMatrix(model);
    }
    else model = Matrix4(1.0f);

    if(!m_ignoreViewMatrix)
    {
        cam->GetViewMatrix(view);
    }
    else view = Matrix4(1.0f);

    if(!m_ignoreProjectionMatrix)
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
    p_material->p_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Model, model, false);
    p_material->p_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Model_Inverse, model.Inversed(), false);
    p_material->p_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_View, view, false);
    p_material->p_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_View_Inverse, view.Inversed(), false);
    p_material->p_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Projection, projection, false);
    p_material->p_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Projection_Inverse, projection.Inversed(), false);
    p_material->p_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_PVM, pvm, false);
}

void Renderer::SetDrawWireframe(bool drawWireframe)
{
    m_drawWireframe = drawWireframe;
}
bool Renderer::GetDrawWireframe() const
{
    return m_drawWireframe;
}


void Renderer::SetCullMode(Renderer::CullMode cullMode)
{
    m_cullMode = cullMode;
}
Renderer::CullMode Renderer::GetCullMode() const
{
    return m_cullMode;
}


void Renderer::SetRenderMode(Renderer::RenderMode renderMode)
{
    m_renderMode = renderMode;
}
Renderer::RenderMode Renderer::GetRenderMode() const
{
    return m_renderMode;
}


void Renderer::SetLineWidth(float w)
{
    m_lineWidth = w;
}
float Renderer::GetLineWidth() const
{
    return m_lineWidth;
}

void Renderer::SetIgnoreModelMatrix(bool ignore)
{
    this->m_ignoreModelMatrix = ignore;
}

bool Renderer::GetIgnoreModelMatrix() const
{
    return m_ignoreModelMatrix;
}

void Renderer::SetIgnoreViewMatrix(bool ignore)
{
    m_ignoreViewMatrix = ignore;
}

bool Renderer::GetIgnoreViewMatrix() const
{
    return m_ignoreViewMatrix;
}

void Renderer::SetIgnoreProjectionMatrix(bool ignore)
{
    m_ignoreProjectionMatrix = ignore;
}

bool Renderer::GetIgnoreProjectionMatrix() const
{
    return m_ignoreProjectionMatrix;
}

void Renderer::SetReceivesLighting(bool receivesLighting)
{
    m_receivesLighting = receivesLighting;
}

bool Renderer::GetReceivesLighting() const
{
    return m_receivesLighting;
}

void Renderer::SetActivateGLStatesBeforeRenderingForSelectionFunction(const std::function<void()> &f)
{
    ActivateGLStatesBeforeRenderingForSelection = f;
}
