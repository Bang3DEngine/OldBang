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
    m_shadowMapFramebuffer->CreateAttachment(GL::Attachment::DepthStencil,
                                             GL::ColorFormat::Depth24_Stencil8);
}

DirectionalLight::~DirectionalLight()
{
    delete m_shadowMapFramebuffer;
}

#include "Bang/Input.h"
void DirectionalLight::RenderShadowMaps()
{
    // Save previous state
    AARecti prevVP = GL::GetViewportRect();
    const Matrix4 &prevModel = GLUniforms::GetModelMatrix();
    const Matrix4 &prevView  = GLUniforms::GetViewMatrix();
    const Matrix4 &prevProj  = GLUniforms::GetProjectionMatrix();
    GLId prevBoundFB = GL::GetBoundId(m_shadowMapFramebuffer->GetGLBindTarget());

    // Bind and resize shadow map framebuffer
    m_shadowMapFramebuffer->Bind();
    m_shadowMapFramebuffer->Resize(GL::GetViewportSize().x, GL::GetViewportSize().y);

    // Set up viewport
    GL::SetViewport(0, 0, prevVP.GetWidth(), prevVP.GetHeight());

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
    if (Input::GetKey(Key::L)) m_shadowMapFramebuffer->ExportDepth(Path("test.png"));

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
    sp->Set("B_LightShadowBias", 0.01f, true);
    sp->Set("B_LightShadowMap", GetShadowMap(), true);
    sp->Set("B_WorldToShadowMapMatrix", GetShadowMapMatrix(scene), true);
}

Texture2D *DirectionalLight::GetShadowMap() const
{
    return m_shadowMapFramebuffer->GetAttachmentTexture(GL::Attachment::DepthStencil);
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
    Transform *trans = GetGameObject()->GetTransform();
    AABox orthoBox = GetShadowMapOrthoBox(scene);
    Vector3 extents = orthoBox.GetExtents();

    *viewMatrix = Matrix4::LookAt(orthoBox.GetCenter(),
                                  orthoBox.GetCenter() + trans->GetForward(),
                                  trans->GetUp());

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
    /*Quaternion rot = Transform::GetRotationFromMatrix4(lookAt);
    Vector3 eulerAng = rot.GetEulerAngles();
    eulerAng.z = 0.0f;
    Quaternion plainRot = Quaternion::FromEulerAngles(eulerAng);
    Vector3 plainUp = plainRot * Vector3::Up;
    return Matrix4::LookAt(Vector3::Zero, t->GetForward(), plainUp);
    */
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
