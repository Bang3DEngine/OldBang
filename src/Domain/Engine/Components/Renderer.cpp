#include "Renderer.h"

#include <functional>

#include "Box.h"
#include "Bang.h"
#include "Scene.h"
#include "Camera.h"
#include "GBuffer.h"
#include "Material.h"
#include "Transform.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "AssetsManager.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"
#include "GraphicPipeline.h"

#ifdef BANG_EDITOR
#include "EditorScene.h"
#include "SelectionFramebuffer.h"
#endif

Renderer::Renderer()
{
    #ifdef BANG_EDITOR
    ActivateGLStatesBeforeRenderingForSelection = [](){};
    #endif

    m_material = AssetsManager::Load<Material>("Materials/D2G_Default.bmat", true);
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
    r->SetReceivesLighting(GetReceivesLighting());
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

    glLineWidth(m_lineWidth);

    // Set uniforms
    Scene *scene = SceneManager::GetActiveScene();
    Camera *camera = scene->GetCamera();
    bool goodMaterial = mat && mat->GetShaderProgram();
    if (goodMaterial)
    {
        if (camera)
        {
            Transform *t = camera->gameObject->transform;
            ShaderProgram *sp = mat->GetShaderProgram();
            sp->SetUniformVec3(ShaderContract::Uniform_Position_Camera, t->GetPosition(), false);
            sp->SetUniformFloat("B_renderer_receivesLighting", m_receivesLighting ? 1.0f : 0.0f, false);

            #ifdef BANG_EDITOR
            if (gameObject)
            {
                sp->SetUniformFloat("B_gameObject_isSelected", gameObject->IsSelected() ? 1.0f : 0.0f, false);
            }
            #endif
        }

        Matrix4 model, normal, view, projection, pvm;
        GetMatrices(&model, &normal, &view, &projection, &pvm);
        SetMatricesUniforms(mat, model, normal, view, projection, pvm);

        GBuffer *gb = GraphicPipeline::GetActive()->GetGBuffer();
        gb->SetUniformsBeforeRendering(mat);
    }
}

void Renderer::Render() const
{
    // TODO: Transparent renderers must not write to the depth buffer (?)
    if (IsTransparent()) { glDepthMask(GL_FALSE); }

    RenderWithMaterial(m_material);

    if (IsTransparent()) { glDepthMask(GL_TRUE); }
}

void Renderer::RenderCustomPR() const
{
    // To override by child classes if they want to implement some PR pass
}

void Renderer::RenderWithMaterial(Material *mat) const
{
    ActivateGLStatesBeforeRendering(mat);

    #ifdef BANG_EDITOR
    SelectionFramebuffer *sfb = GraphicPipeline::GetActive()->GetSelectionFramebuffer();
    if (sfb && sfb->IsPassing())
    {
        ActivateGLStatesBeforeRenderingForSelection();
    }
    #endif

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

bool Renderer::IsACanvasRenderer() const
{
    return false;
}

void Renderer::SetTransparent(bool transparent)
{
    m_isTransparent = transparent;
}
bool Renderer::IsTransparent() const
{
    return m_isTransparent;
}

void Renderer::SetIsGizmo(bool isGizmo)
{
    m_isGizmo = isGizmo;
}

bool Renderer::IsGizmo() const
{
    return m_isGizmo;
}

bool Renderer::HasCustomPRPass() const
{
    return m_hasCustomPRPass;
}

void Renderer::SetDepthLayer(Renderer::DepthLayer dl)
{
    m_depthLayer = dl;
}

Renderer::DepthLayer Renderer::GetDepthLayer() const
{
    return m_depthLayer;
}

void Renderer::GetMatrices(Matrix4 *model,
                           Matrix4 *normal,
                           Matrix4 *view,
                           Matrix4 *projection,
                           Matrix4 *pvm) const
{
    //We assume cam, scene and transform do exist.
    Camera *cam = SceneManager::GetActiveScene()->GetCamera();
    if (gameObject)
    {
        gameObject->transform->GetLocalToWorldMatrix(model);
        gameObject->transform->GetLocalToWorldNormalMatrix(normal);
    }
    else
    {
        *model = *normal = Matrix4::Identity;
    }

    cam->GetViewMatrix(view);
    cam->GetProjectionMatrix(projection);
    *pvm = (*projection) * (*view) * (*model);
}

void Renderer::SetMatricesUniforms(Material *mat,
                                   const Matrix4 &model,
                                   const Matrix4 &normal,
                                   const Matrix4 &view,
                                   const Matrix4 &projection,
                                   const Matrix4 &pvm) const
{
    NONULL(mat); NONULL(mat->m_shaderProgram);

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

Rect Renderer::GetBoundingRect(Camera *camera) const
{
    Camera *cam = camera ? camera : SceneManager::GetActiveScene()->GetCamera();
    Vector3 camForward = cam->transform->GetForward();

    Box bb = GetBoundingBox();

    List<Vector3> points = bb.GetPoints();
    bool allPointsOutside = true;
    for (const Vector3 &p : points)
    {
        if (Vector3::Dot(p, camForward) > 0)
        {
            allPointsOutside = false;
            break;
        }
    }

    if (allPointsOutside)
    {
        return Rect::Empty;
    }

    if (gameObject)
    {
        return bb.GetBoundingScreenRect(cam,
                                        gameObject->transform->GetPosition(),
                                        gameObject->transform->GetRotation(),
                                        gameObject->transform->GetScale()
                                        );
    }
    else
    {
        return bb.GetBoundingScreenRect(cam);
    }
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
        SetMaterial( AssetsManager::Load<Material>(materialFilepath) );
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
