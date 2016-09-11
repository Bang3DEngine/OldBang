#include "Renderer.h"

#include "Scene.h"
#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"
#include "GraphicPipeline.h"

#ifdef BANG_EDITOR
#include "EditorScene.h"
#include "SelectionFramebuffer.h"
#endif

Renderer::Renderer()
{
    ActivateGLStatesBeforeRenderingForSelection = [](){};
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
    r->SetIgnoreModelMatrix(GetIgnoreModelMatrix());
    r->SetIgnoreViewMatrix(GetIgnoreViewMatrix());
    r->SetIgnoreProjectionMatrix(GetIgnoreProjectionMatrix());
}

Material *Renderer::GetMaterial() const
{
    return m_material;
}

void Renderer::ActivateGLStatesBeforeRendering(Material *mat) const
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
    if (camera && mat && mat->GetShaderProgram())
    {
        Transform *t = camera->gameObject->transform;
        ShaderProgram *sp = mat->GetShaderProgram();
        sp->SetUniformVec3(ShaderContract::Uniform_Position_Camera, t->GetPosition(), false);
        sp->SetUniformFloat("B_renderer_receivesLighting", m_receivesLighting ? 1.0f : 0.0f, false);

        #ifdef BANG_EDITOR
        sp->SetUniformFloat("B_gameObject_isSelected", gameObject->IsSelectedInHierarchy() ? 1.0f : 0.0f, false);
        #endif
    }

    glLineWidth(m_lineWidth);
}

void Renderer::Render() const
{
    #ifdef BANG_EDITOR
    SelectionFramebuffer *sfb = GraphicPipeline::GetActive()->GetSelectionFramebuffer();
    if (sfb && sfb->IsPassing())
    {   // For SelectionFramebuffer OnDrawGizmos(NoDepth)
        RenderWithMaterial( sfb->GetSelectionMaterial() ); // RENDER FOR FRAMEBUFFER
    }
    else
    #endif
    {
        RenderWithMaterial(m_material);
    }
}

void Renderer::RenderWithMaterial(Material *mat) const
{
    ActivateGLStatesBeforeRendering(mat);
    SelectionFramebuffer *sfb = GraphicPipeline::GetActive()->GetSelectionFramebuffer();
    if (sfb && sfb->IsPassing())
    {
        ActivateGLStatesBeforeRenderingForSelection();
    }

    Matrix4 model, normal, view, projection, pvm;
    GetMatrices(&model, &normal, &view, &projection, &pvm);
    SetMatricesUniforms(mat, model, normal, view, projection, pvm);

    bool goodMaterial = mat && mat->GetShaderProgram();
    if (goodMaterial)
    {
        mat->Bind();
    }

    RenderWithoutBindingMaterial();

    if (goodMaterial)
    {
        mat->UnBind();
    }
}

void Renderer::SetTransparent(bool transparent)
{
    m_isTransparent = transparent;
}
bool Renderer::IsTransparent() const
{
    return m_isTransparent;
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

void Renderer::SetMatricesUniforms(Material *mat,
                                   const Matrix4 &model,
                                   const Matrix4 &normal,
                                   const Matrix4 &view,
                                   const Matrix4 &projection,
                                   const Matrix4 &pvm) const
{
    ShaderProgram *sp = mat->m_shaderProgram;
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

    SetTransparent(xmlInfo->GetBool("IsTransparent"));
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
    xmlInfo->SetBool("IsTransparent", IsTransparent(), {XMLProperty::Inline});
    xmlInfo->SetBool("DrawWireframe", GetDrawWireframe(), {XMLProperty::Inline});
    xmlInfo->SetBool("ReceivesLighting", GetReceivesLighting(), {XMLProperty::Inline});
}
