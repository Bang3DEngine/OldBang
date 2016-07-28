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
    r->SetMaterial(m_material);
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
    return m_material;
}

void Renderer::ActivateGLStatesBeforeRendering() const
{
    //Set polygon mode
    if (m_drawWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //Set culling mode
    if (m_cullMode != CullMode::None)
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
    if (camera  && m_material  && m_material->m_shaderProgram )
    {
        Transform *t = camera->gameObject->transform;
        m_material->m_shaderProgram->SetUniformVec3(ShaderContract::Uniform_Position_Camera,
                                                    t->GetPosition(), false);

        m_material->m_shaderProgram->SetUniformFloat("B_renderer_receivesLighting",
                                                      m_receivesLighting ? 1.0f : 0.0f, false);

        #ifdef BANG_EDITOR
        m_material->m_shaderProgram->SetUniformFloat("B_gameObject_isSelected",
                                                      gameObject->IsSelectedInHierarchy() ? 1.0f : 0.0f, false);
        #endif
    }

    glLineWidth(m_lineWidth);
}

void Renderer::OnRender()
{
    Render();
}

void Renderer::Render() const
{
    NONULL(m_material); NONULL(m_material->GetShaderProgram());

    ActivateGLStatesBeforeRendering();
    m_material->Bind();
    RenderWithoutBindingMaterial();
    m_material->UnBind();
}

void Renderer::GetMatrices(Matrix4 *model,
                           Matrix4 *normal,
                           Matrix4 *view,
                           Matrix4 *projection,
                           Matrix4 *pvm) const
{
    //We assume cam, scene and transform do exist.

    Camera *cam = gameObject->GetScene()->GetCamera();

    if (!m_ignoreModelMatrix)
    {
        gameObject->transform->GetModelMatrix(model);
        gameObject->transform->GetNormalMatrix(normal);
    }
    else
    {
        *model = Matrix4(1.0f);
        *normal = Matrix4(1.0f);
    }

    if (!m_ignoreViewMatrix)
    {
        cam->GetViewMatrix(view);
    }
    else
    {
        *view = Matrix4(1.0f);
    }

    if (!m_ignoreProjectionMatrix)
    {
        cam->GetProjectionMatrix(projection);
    }
    else
    {
        *projection = Matrix4(1.0f);
    }

    *pvm = (*projection) * (*view) * (*model);
}

void Renderer::SetMatricesUniforms(const Matrix4 &model,
                                   const Matrix4 &normal,
                                   const Matrix4 &view,
                                   const Matrix4 &projection,
                                   const Matrix4 &pvm) const
{
    m_material->m_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Model, model, false);
    m_material->m_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Model_Inverse, model.Inversed(), false);
    m_material->m_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Normal, normal, false);
    m_material->m_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Normal_Inverse, normal.Inversed(), false);
    m_material->m_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_View, view, false);
    m_material->m_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_View_Inverse, view.Inversed(), false);
    m_material->m_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Projection, projection, false);
    m_material->m_shaderProgram->SetUniformMat4(
                ShaderContract::Uniform_Matrix_Projection_Inverse, projection.Inversed(), false);
    m_material->m_shaderProgram->SetUniformMat4(
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

#ifdef BANG_EDITOR
void Renderer::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void Renderer::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}
#endif

void Renderer::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);

    std::string materialFilepath = xmlInfo->GetFilepath("Material");
    if (materialFilepath != "-")
    {
        SetMaterial( AssetsManager::GetAsset<Material>(materialFilepath) );
    }
    else
    {
        SetMaterial (nullptr);
    }

    SetLineWidth(xmlInfo->GetFloat("LineWidth"));
}

void Renderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Renderer");

    if (m_material)
    {
        if (m_material->GetFilepath() != "")
        {
            xmlInfo->SetFilepath("Material", m_material->GetFilepath());
        }
        else //In case the asset is created in runtime, write its mem address
        {
            std::string memAddress;
            Logger_GetString(memAddress, (void*)m_material);
            xmlInfo->SetFilepath("Material", memAddress);
        }
    }
    else
    {
        xmlInfo->SetFilepath("Material", "-");
    }

    xmlInfo->SetFloat("LineWidth", GetLineWidth());
}
