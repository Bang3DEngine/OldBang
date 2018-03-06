#include "Bang/DirectionalLight.h"

#include "Bang/AABox.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Gizmos.h"
#include "Bang/GEngine.h"
#include "Bang/XMLNode.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/Transform.h"
#include "Bang/GLUniforms.h"
#include "Bang/IconManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/DebugRenderer.h"
#include "Bang/MaterialFactory.h"

USING_NAMESPACE_BANG

DirectionalLight::DirectionalLight()
{
    SetLightMaterial(MaterialFactory::GetDirectionalLight().Get());

    m_shadowMapFramebuffer = new Framebuffer(1,1);
    m_shadowMapFramebuffer->CreateAttachment(GL::Attachment::Depth,
                                             GL::ColorFormat::Depth16);

    GLId prevBoundTex = GL::GetBoundId(GetShadowMap()->GetGLBindTarget());
    GetShadowMap()->Bind();
    GetShadowMap()->SetFilterMode(GL::FilterMode::Bilinear);
    GL::TexParameteri( GetShadowMap()->GetTextureTarget(),
                       GL::TexParameter::TEXTURE_COMPARE_MODE,
                       GL_COMPARE_REF_TO_TEXTURE );
    GL::Bind(GetShadowMap()->GetGLBindTarget(), prevBoundTex);
}

DirectionalLight::~DirectionalLight()
{
    delete m_shadowMapFramebuffer;
}

void DirectionalLight::RenderShadowMaps_()
{
    // Save previous state
    AARecti prevVP = GL::GetViewportRect();
    const Matrix4 &prevModel = GLUniforms::GetModelMatrix();
    const Matrix4 &prevView  = GLUniforms::GetViewMatrix();
    const Matrix4 &prevProj  = GLUniforms::GetProjectionMatrix();
    GLId prevBoundFB = GL::GetBoundId(m_shadowMapFramebuffer->GetGLBindTarget());

    // Bind and resize shadow map framebuffer
    Vector2i shadowMapSize = Vector2i(2048);
    m_shadowMapFramebuffer->Bind();
    m_shadowMapFramebuffer->Resize(shadowMapSize.x, shadowMapSize.y);

    // Set up viewport
    GL::SetViewport(0, 0, shadowMapSize.x, shadowMapSize.y);

    // Set up shadow map matrices
    Scene *scene = GetGameObject()->GetScene();
    Matrix4 shadowMapViewMatrix, shadowMapProjMatrix;
    GetShadowMapMatrices(scene, &shadowMapViewMatrix, &shadowMapProjMatrix);
    GLUniforms::SetModelMatrix(Matrix4::Identity);
    GLUniforms::SetViewMatrix( shadowMapViewMatrix );
    GLUniforms::SetProjectionMatrix( shadowMapProjMatrix );

    // Render shadow map into framebuffer
    GL::ClearDepthBuffer(1.0f);
    GL::SetColorMask(false, false, false, false);
    GL::SetDepthFunc(GL::Function::LEqual);
    GL::SetDepthMask(true);
    GEngine::GetActive()->RenderWithPassRaw(scene, RenderPass::Scene);

    // Restore previous state
    GL::SetViewport(prevVP);
    GL::SetColorMask(true, true, true, true);
    GLUniforms::SetModelMatrix(prevModel);
    GLUniforms::SetViewMatrix(prevView);
    GLUniforms::SetProjectionMatrix(prevProj);
    GL::Bind(m_shadowMapFramebuffer->GetGLBindTarget(), prevBoundFB);
}

void DirectionalLight::SetUniformsBeforeApplyingLight(Material *mat) const
{
    Light::SetUniformsBeforeApplyingLight(mat);

    ShaderProgram *sp = mat->GetShaderProgram();
    if (!sp) { return; }
    ASSERT(GL::IsBound(sp))

    Scene *scene = GetGameObject()->GetScene();
    sp->Set("B_LightShadowMap", GetShadowMap(), true);
    sp->Set("B_WorldToShadowMapMatrix", GetShadowMapMatrix(scene), true);
}

Texture2D *DirectionalLight::GetShadowMap() const
{
    return m_shadowMapFramebuffer->GetAttachmentTexture(GL::Attachment::Depth);
}

void DirectionalLight::CloneInto(ICloneable *clone) const
{
    Light::CloneInto(clone);
}

void DirectionalLight::GetShadowMapMatrices(Scene *scene,
                                            Matrix4 *viewMatrix,
                                            Matrix4 *projMatrix) const
{
    // The ortho box will be the AABox in light space of the AABox of the
    // scene in world space
    AABox orthoBox = GetShadowMapOrthoBox(scene);
    Vector3 extents = orthoBox.GetExtents();
    Matrix4 lightDirMatrixInv = GetLightDirMatrix().Inversed();
    Vector3 fwd = lightDirMatrixInv.TransformVector(Vector3::Forward);
    Vector3 up  = lightDirMatrixInv.TransformVector(Vector3::Up);

    *viewMatrix = Matrix4::LookAt(orthoBox.GetCenter(),
                                  orthoBox.GetCenter() + fwd,
                                  up);

    *projMatrix = Matrix4::Ortho(-extents.x, extents.x,
                                 -extents.y, extents.y,
                                 -extents.z, extents.z);
}

Matrix4 DirectionalLight::GetShadowMapMatrix(Scene *scene) const
{
    Matrix4 shadowMapViewMatrix, shadowMapProjMatrix;
    GetShadowMapMatrices(scene, &shadowMapViewMatrix, &shadowMapProjMatrix);
    return shadowMapProjMatrix * shadowMapViewMatrix;
}


AABox DirectionalLight::GetShadowMapOrthoBox(Scene *scene) const
{
    // Get scene AABox
    const AABox sceneAABox = scene->GetAABBox(true);
    const Array<Vector3> sceneBoxPoints = sceneAABox.GetPoints();

    // Get light space matrix
    const Matrix4 lightMatrixInv = GetLightDirMatrix().Inversed();

    // Construct an AABox of the sceneAABox points in light space
    // (AABox_in_light_space of the scene AABox_in_world_space)
    AABox orthoBox;
    for (const Vector3 &sceneBoxPoint : sceneBoxPoints)
    {
        Vector3 sceneBoxPointInLightSpace =
              (lightMatrixInv * Vector4(sceneBoxPoint, 1)).xyz();
        orthoBox.AddPoint( sceneBoxPointInLightSpace );
    }
    return orthoBox;
}

Matrix4 DirectionalLight::GetLightDirMatrix() const
{
    const Transform *t = GetGameObject()->GetTransform();
    Matrix4 lookAt = Matrix4::LookAt(Vector3::Zero, t->GetForward(), t->GetUp());
    return lookAt;
}

void DirectionalLight::OnRender(RenderPass rp)
{
    Component::OnRender(rp);

    // Gizmo rendering
    if (rp == RenderPass::Overlay)
    {
        Gizmos::Reset();
        Gizmos::SetColor(GetColor());
        Gizmos::SetSelectable(GetGameObject());
        Gizmos::SetPosition( GetGameObject()->GetTransform()->GetPosition() );
        Gizmos::SetScale( Vector3(0.1f) );
        Gizmos::RenderIcon( IconManager::GetSunIcon().Get(), true );
    }
}

void DirectionalLight::ImportXML(const XMLNode &xmlInfo)
{
    Light::ImportXML(xmlInfo);
}

void DirectionalLight::ExportXML(XMLNode *xmlInfo) const
{
    Light::ExportXML(xmlInfo);
}
