#include "Bang/DirectionalLight.h"

#include "Bang/Quad.h"
#include "Bang/AABox.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Gizmos.h"
#include "Bang/GEngine.h"
#include "Bang/XMLNode.h"
#include "Bang/Geometry.h"
#include "Bang/Triangle.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/Transform.h"
#include "Bang/GLUniforms.h"
#include "Bang/IconManager.h"
#include "Bang/ShadowMapper.h"
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

#include "Bang/Input.h"
void DirectionalLight::RenderShadowMaps_()
{
    // Save previous state
    AARecti prevVP = GL::GetViewportRect();
    bool wasDepthClampEnabled = GL::IsEnabled(GL::Test::DepthClamp);
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
    m_lastUsedShadowMapViewProj = shadowMapProjMatrix * shadowMapViewMatrix;
    if (Input::GetKey(Key::LShift) && Input::GetKeyDown(Key::L))
    {
        m_shadowMapFramebuffer->ExportDepth(Path("test.png"));
    }

    // Render shadow map into framebuffer
    // GL::Enable(GL::Test::DepthClamp);
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
    GL::SetEnabled(GL::Test::DepthClamp, wasDepthClampEnabled);
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
    sp->Set("B_LightShadowMapSoft", GetShadowMap(), true);
    sp->Set("B_WorldToShadowMapMatrix", m_lastUsedShadowMapViewProj, true);
}

void DirectionalLight::SetShadowDistance(float shadowDistance)
{
    m_shadowDistance = shadowDistance;
}

float DirectionalLight::GetShadowDistance() const
{
    return m_shadowDistance;
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
    AABox orthoBoxInLightSpace = GetShadowMapOrthoBox(scene);
    Vector3 orthoBoxExtents = orthoBoxInLightSpace.GetExtents();
    Matrix4 lightDirMatrix    = GetLightDirMatrix();
    Matrix4 lightDirMatrixInv = lightDirMatrix.Inversed();
    Vector3 fwd = lightDirMatrixInv.TransformVector(Vector3::Forward);
    Vector3 up  = lightDirMatrixInv.TransformVector(Vector3::Up);

    Vector3 orthoBoxCenterWorld =
         lightDirMatrix.TransformPoint( orthoBoxInLightSpace.GetCenter() );
    *viewMatrix = Matrix4::LookAt(orthoBoxCenterWorld,
                                  orthoBoxCenterWorld + fwd,
                                  up);

    *projMatrix = Matrix4::Ortho(-orthoBoxExtents.x,  orthoBoxExtents.x,
                                 -orthoBoxExtents.y,  orthoBoxExtents.y,
                                 -orthoBoxExtents.z,  orthoBoxExtents.z);
}

Matrix4 DirectionalLight::GetShadowMapMatrix(Scene *scene) const
{
    Matrix4 shadowMapViewMatrix, shadowMapProjMatrix;
    GetShadowMapMatrices(scene, &shadowMapViewMatrix, &shadowMapProjMatrix);
    return shadowMapProjMatrix * shadowMapViewMatrix;
}

AABox DirectionalLight::GetShadowMapOrthoBox(Scene *scene) const
{
    // Adjust zFar so that we take into account shadow distance, and get our
    // camera frustum quads
    Camera *cam = Camera::GetActive(); // Get active camera
    float prevZFar = cam->GetZFar();   // Save for later restore
    cam->SetZFar( Math::Min(prevZFar, GetShadowDistance()) ); // Shadow distance
    const Quad camTopQuad   = cam->GetTopQuad();    // Get top quad
    const Quad camBotQuad   = cam->GetBotQuad();    // Get bot quad
    const Quad camLeftQuad  = cam->GetLeftQuad();   // Get left quad
    const Quad camRightQuad = cam->GetRightQuad();  // Get right quad
    const std::array<Quad, 4> camQuads = {camTopQuad, camBotQuad,
                                          camLeftQuad, camRightQuad};
    cam->SetZFar(prevZFar); // Restore

    Array<Vector3> camFrustumPoints;
    for (const Quad &quad : camQuads)
    {
        for (const Vector3 &p : quad.GetPoints())
        { camFrustumPoints.PushBack(p); }
    }

    /*
    AABox camFrustumAABox( camQuads.front().GetPoint(0) );
    for (const Quad& camQuad : camQuads)
    {
        for (const Vector3 &points : camQuad.GetPoints())
        {
            camFrustumAABox.AddPoint(points);
        }
    }
    const Array<Vector3> camFrustumBoxPoints = camFrustumAABox.GetPoints();

    AABox pointsToBeInsideShadowMapAABox;
    for (const Vector3 &pointToBeInsideShadowMap : pointsToBeInsideShadowMap)
    {
        pointsToBeInsideShadowMapAABox.AddPoint(pointToBeInsideShadowMap);
    }
    */

    // Get light space matrix
    const Matrix4 lightMatrix    = GetLightDirMatrix();
    const Matrix4 lightMatrixInv = lightMatrix.Inversed();

    // Get the orthogonal aabox in light space (aligned with light direction)
    // that contains our camera frustum
    AABox camOrthoBoxInLightSpace;
    for (const Vector3 &camFrustumPoint : camFrustumPoints)
    {
        Vector3 camBoxPointInLightSpace =
                lightMatrixInv.TransformPoint(camFrustumPoint);
        camOrthoBoxInLightSpace.AddPoint( camBoxPointInLightSpace );
    }

    // Extend light box in y and z back and forth, so that we can intersect in
    // the next step
    constexpr float Ext = 9999999;
    AABox extCamOrthoBoxInLightSpace;
    extCamOrthoBoxInLightSpace.SetMin( camOrthoBoxInLightSpace.GetMin() -
                                       Vector3(0, 0, Ext) );
    extCamOrthoBoxInLightSpace.SetMax( camOrthoBoxInLightSpace.GetMax() +
                                       Vector3(0, 0, Ext) );

    // Now, intersect the very long top, bot, left and right shadow map ortho box with
    // the scene AABBox, so that we can determine the optimal near and far distances
    // for our shadow map ortho box! (so that all affecting shadow casters are inside)
    Quad orthoBoxTopQuadWorldSpace   = lightMatrix * extCamOrthoBoxInLightSpace.GetTopQuad();
    Quad orthoBoxBotQuadWorldSpace   = lightMatrix * extCamOrthoBoxInLightSpace.GetBotQuad();
    Quad orthoBoxLeftQuadWorldSpace  = lightMatrix * extCamOrthoBoxInLightSpace.GetLeftQuad();
    Quad orthoBoxRightQuadWorldSpace = lightMatrix * extCamOrthoBoxInLightSpace.GetRightQuad();

    // Get scene AABBox and intersect with all extended quads!
    const AABox sceneAABox = ShadowMapper::GetSceneCastersAABox(scene);
    Array<Vector3> camQuadsLSSceneBoxIntersections;
    camQuadsLSSceneBoxIntersections.PushBack(
         Geometry::IntersectQuadAABox(orthoBoxTopQuadWorldSpace,  sceneAABox));
    // camQuadsLSSceneBoxIntersections.PushBack(
    //      Geometry::IntersectQuadAABox(orthoBoxBotQuadWorldSpace,  sceneAABox));
    // camQuadsLSSceneBoxIntersections.PushBack(
    //      Geometry::IntersectQuadAABox(orthoBoxRightQuadWorldSpace,  sceneAABox));
    // camQuadsLSSceneBoxIntersections.PushBack(
    //      Geometry::IntersectQuadAABox(orthoBoxRightQuadWorldSpace,  sceneAABox));

    // Make an array of all the points we want to have inside our shadow map
    // First of all, we want to have the whole camera frustum inside of it
    // And then, we want to have all the shadow casters that affect us too
    float time = 20.0f;
    if (Input::GetKeyDown(Key::I))
    {
        int limits = 10;
        auto GetRand = [limits](){ return (rand() % limits) - limits; };

        Vector3 s0  = Vector3(10,  10, 10);
        Vector3 s1  = Vector3(-10, -10, -10);
        Triangle tri0 = Triangle(
                    Vector3(-10, -10, 0),
                    Vector3(-10,  10, 0),
                    Vector3( 10,  10, 0)
                                 );
        Triangle tri1 = Triangle(
                    Vector3(0, -10 + 5, -10),
                    Vector3(0, -10 + 5,  10),
                    Vector3(0,  10 + 5,  10)
                                 );

        Vector3 p0, p1;
        int numIntersectionPoints;
        Geometry::IntersectTriangleTriangle(tri0, tri1, &numIntersectionPoints, &p0, &p1);
        // DebugRenderer::RenderLine(s0, s1, Color::Pink, time, 3.0f, true);
        DebugRenderer::RenderTriangle(tri0, Color::Blue, time, false, false, true);
        DebugRenderer::RenderTriangle(tri1, Color::DarkBlue, time, false, false, true);
        if (numIntersectionPoints >= 1) { DebugRenderer::RenderPoint(p0, Color::Red, time, 10.0f, false); }
        if (numIntersectionPoints >= 2) { DebugRenderer::RenderPoint(p1, Color::Red, time, 10.0f, false); }
        Debug_Peek(numIntersectionPoints);
        Debug_Peek(p0);
        Debug_Peek(p1);
        Debug_Log("==========");
    }

    if (Input::GetKeyDown(Key::Q))
    {
        Debug_Peek(camQuadsLSSceneBoxIntersections);
        DebugRenderer::RenderQuad(orthoBoxTopQuadWorldSpace, Color::Blue, time, false, false, true);
        DebugRenderer::RenderQuad(orthoBoxBotQuadWorldSpace, Color::Blue, time, false, false, true);

        for (const Vector3 &intPoint : camQuadsLSSceneBoxIntersections)
        {
            DebugRenderer::RenderPoint(intPoint, Color::Red, time, 10.0f, false);
        }
        // DebugRenderer::RenderQuad(orthoBoxLeftQuadWorldSpace, Color::Blue, time, false, false, true);
        // DebugRenderer::RenderQuad(orthoBoxRightQuadWorldSpace, Color::Blue, time, false, false, true);
    }

    /*
    for (const Quad &quad : {camLeftQuad, camRightQuad})
    {
        int numIntersectionPoints;
        std::array<Vector3, 4> intersectionPoints;
        Geometry::IntersectQuadAABox(quad,
                                     sceneAABox,
                                     &numIntersectionPoints,
                                     &intersectionPoints[0],
                                     &intersectionPoints[1],
                                     &intersectionPoints[2],
                                     &intersectionPoints[3]);
        for (int i = 0; i < numIntersectionPoints; ++i)
        {
            pointsToBeInsideShadowMap.PushBack(intersectionPoints[i]);
        }
    }
    */

/*
    // Readjust ortho box to fit better
    Vector3 orthoBoxCenter = orthoBoxInLightSpace.GetCenter();
    Vector3 extents = orthoBoxInLightSpace.GetExtents();
    // extents.z = GetShadowDistance() * 0.5f;
    orthoBoxInLightSpace = AABox(orthoBoxCenter - extents,
                                 orthoBoxCenter + extents);
    */

    AABox orthoBoxInLightSpace = camOrthoBoxInLightSpace;

    if (Input::GetKey(Key::C))
    {
        Camera *cam = scene->GetCamera();
        auto quads = {cam->GetNearQuad(), cam->GetFarQuad(),
                      cam->GetLeftQuad(), cam->GetRightQuad(),
                      cam->GetTopQuad(), cam->GetBotQuad()};
        for (const Quad &quad : quads)
        {
            DebugRenderer::RenderQuad(quad, Color::Purple, 0.1f, true, false, true);
        }
    }

    int j = 0;
    if (Input::GetKey(Key::L))
    {
        auto quads = orthoBoxInLightSpace.GetQuads();
        for (const Quad &quad : quads)
        {
            Quad worldQuad (lightMatrix.TransformPoint(quad.GetPoint(0)),
                            lightMatrix.TransformPoint(quad.GetPoint(1)),
                            lightMatrix.TransformPoint(quad.GetPoint(2)),
                            lightMatrix.TransformPoint(quad.GetPoint(3)));

            DebugRenderer::RenderQuad(worldQuad, (j == 4) ? Color::Green : Color::Red,
                                      0.1f, true, false, true);
            for (int i : {0,1,2,3})
            {
                DebugRenderer::RenderPoint(worldQuad.GetPoint(i), Color::Green,
                                           0.1f, 5.0f, true);
            }
            ++j;
        }
    }

    if (Input::GetKey(Key::S))
    {
        for (const Quad &quad : sceneAABox.GetQuads())
        {
            DebugRenderer::RenderQuad(quad, Color::Pink, 0.1f, true, false, true);
        }
    }

    return orthoBoxInLightSpace;
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

    if (xmlInfo.Contains("ShadowDistance"))
    { SetShadowDistance(xmlInfo.Get<float>("ShadowDistance")); }
}

void DirectionalLight::ExportXML(XMLNode *xmlInfo) const
{
    Light::ExportXML(xmlInfo);

    xmlInfo->Set("ShadowDistance", GetShadowDistance());
}
