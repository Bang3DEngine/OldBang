#include "Renderer.h"

#include "Camera.h"
#include "Scene.h"
#include "Transform.h"
#include "GameObject.h"

#ifdef BANG_EDITOR
#include "EditorScene.h"
#include "SelectionFramebuffer.h"
#endif

Renderer::Renderer()
{
}

void Renderer::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Renderer *r = static_cast<Renderer*>(clone);
    Component::CloneInto(r);
    r->SetMaterial(GetMaterial());
    r->SetDrawWireframe(GetDrawWireframe());
    r->SetCullMode(GetCullMode());
    r->SetRenderMode(GetRenderMode());
    r->SetLineWidth(GetLineWidth());
    r->SetIgnoreMaterial(GetIgnoreMaterial());
    r->SetIgnoreModelMatrix(GetIgnoreModelMatrix());
    r->SetIgnoreViewMatrix(GetIgnoreViewMatrix());
    r->SetIgnoreProjectionMatrix(GetIgnoreProjectionMatrix());
}

Material *Renderer::GetMaterial() const
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
        glCullFace(GLint(m_cullMode));
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    Scene *scene = SceneManager::GetActiveScene();
    Camera *camera = scene->GetCamera();
    if (camera && m_material  && m_material->GetShaderProgram())
    {
        Transform *t = camera->gameObject->transform;
        ShaderProgram *sp = m_material->GetShaderProgram();
        sp->SetUniformVec3(ShaderContract::Uniform_Position_Camera, t->GetPosition(), false);
        sp->SetUniformFloat("B_renderer_receivesLighting", m_receivesLighting ? 1.0f : 0.0f, false);

        #ifdef BANG_EDITOR
        sp->SetUniformFloat("B_gameObject_isSelected", gameObject->IsSelectedInHierarchy() ? 1.0f : 0.0f, false);
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
    #ifdef BANG_EDITOR
    EditorScene *scene = static_cast<EditorScene*>(SceneManager::GetActiveScene());
    SelectionFramebuffer *sfb = scene->GetSelectionFramebuffer();
    if (sfb && sfb->IsPassing())
    {
        RenderSelectionFramebuffer(sfb);
    }
    else
    #endif
    {
        ActivateGLStatesBeforeRendering();

        if (!GetIgnoreMaterial())
        {
            NONULL(m_material);
            NONULL(m_material->GetShaderProgram());
            m_material->Bind();
        }

        RenderWithoutBindingMaterial();

        if (!GetIgnoreMaterial())
        {
            m_material->UnBind();
        }
    }
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
        gameObject->transform->GetLocalToWorldMatrix(model);
        gameObject->transform->GetLocalToWorldNormalMatrix(normal);
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
    ShaderProgram *sp = m_material->m_shaderProgram;
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_Model, model, false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_Model_Inverse, model.Inversed(), false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_Normal, normal, false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_Normal_Inverse, normal.Inversed(), false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_View, view, false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_View_Inverse, view.Inversed(), false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_Projection, projection, false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_Projection_Inverse, projection.Inversed(), false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_PVM, pvm, false);
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

void Renderer::SetIgnoreMaterial(bool ignore)
{
    m_ignoreMaterial = ignore;
}

bool Renderer::GetIgnoreMaterial() const
{
    return m_ignoreMaterial;
}

void Renderer::SetIgnoreModelMatrix(bool ignore)
{
    m_ignoreModelMatrix = ignore;
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

#ifdef BANG_EDITOR

void Renderer::RenderSelectionFramebuffer(SelectionFramebuffer *sfb) const
{
    Material *selMaterial = sfb->GetSelectionMaterial(); NONULL(selMaterial);
    ShaderProgram *sp = selMaterial->GetShaderProgram();

    Matrix4 model, normal, view, projection, pvm;
    GetMatrices(&model, &normal, &view, &projection, &pvm);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_Model, model, false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_Normal, normal, false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_View, view, false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_Projection, projection, false);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_PVM, pvm, false);

    ActivateGLStatesBeforeRendering();
    if (ActivateGLStatesBeforeRenderingForSelection)
    {
        ActivateGLStatesBeforeRenderingForSelection();
    }

    selMaterial->Bind();

    RenderWithoutBindingMaterial();

    selMaterial->UnBind();
}

void Renderer::SetActivateGLStatesBeforeRenderingForSelectionFunction(const std::function<void()> &f)
{
    ActivateGLStatesBeforeRenderingForSelection = f;
}

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

    String materialFilepath = xmlInfo->GetFilepath("Material");
    if (!materialFilepath.Empty())
    {
        SetMaterial( AssetsManager::LoadAsset<Material>(materialFilepath) );
    }
    else
    {
        SetMaterial (nullptr);
    }

    SetLineWidth(xmlInfo->GetFloat("LineWidth"));
    SetDrawWireframe(xmlInfo->GetBool("DrawWireframe"));
    SetReceivesLighting(xmlInfo->GetBool("ReceivesLighting"));
}

void Renderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Renderer");

    if (m_material)
    {
        if (m_material->GetFilepath() != "")
        {
            xmlInfo->SetFilepath("Material", m_material->GetFilepath(), "bmat");
        }
        else //In case the asset is created in runtime, write its mem address
        {
            String memAddress = String::ToString((void*) m_material);
            xmlInfo->SetFilepath("Material", memAddress, "bmat");
        }
    }
    else
    {
        xmlInfo->SetFilepath("Material", "", "bmat");
    }

    xmlInfo->SetFloat("LineWidth", GetLineWidth());
    xmlInfo->SetBool("DrawWireframe", GetDrawWireframe(), {XMLProperty::Inline});
    xmlInfo->SetBool("ReceivesLighting", GetReceivesLighting(), {XMLProperty::Inline});
}
