#include "Bang/GEngine.h"

#include "Bang/GL.h"
#include "Bang/VAO.h"
#include "Bang/VBO.h"
#include "Bang/Mesh.h"
#include "Bang/Scene.h"
#include "Bang/Light.h"
#include "Bang/Input.h"
#include "Bang/Window.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/ChronoGL.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/Texture2D.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/GLUniforms.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutManager.h"
#include "Bang/MaterialFactory.h"
#include "Bang/TextureUnitManager.h"
#include "Bang/SelectionFramebuffer.h"

USING_NAMESPACE_BANG

GEngine* GEngine::s_gEngine = nullptr;

GEngine::GEngine()
{
}

GEngine::~GEngine()
{
    delete m_texUnitManager;
    delete m_gl;
}

void GEngine::Init()
{
    m_gl = new GL();
    GL::SetActive( GetGL() );
    m_texUnitManager = new TextureUnitManager();

    m_renderGBufferToScreenMaterial = MaterialFactory::GetRenderGBufferToScreen();
    m_screenPlaneMesh = MeshFactory::GetUIPlane();
    GL::SetActive( nullptr );
}

void GEngine::Render(GameObject *go, Camera *camera)
{
    _BindCamera(camera);
    RenderToGBuffer(go, camera);
    RenderToSelectionFramebuffer(go, camera);
    _BindCamera(nullptr);
}

void GEngine::Render(Scene *scene)
{
    List<Camera*> sceneCameras = scene->GetComponentsInChildren<Camera>();
    for (Camera *camera : sceneCameras)
    {
        GameObject *gameObjectToRender = camera->GetGameObjectToRender();
        if (gameObjectToRender)
        {
            Render(gameObjectToRender, camera);
        }
    }
}

void GEngine::ApplyDeferredLights(GameObject *lightsContainer,
                                  Renderer *lightReceiver,
                                  Camera *camera)
{
    Rect maskRect = lightReceiver->GetGameObject()->GetBoundingScreenRect(camera, false);
    maskRect = Rect::ScreenRectNDC; // TAKE THIS OUT !!!!
    ENSURE(maskRect != Rect::Zero);
    ApplyDeferredLightsToGBuffer(lightsContainer, camera, maskRect);
}

void GEngine::ApplyDeferredLights(GameObject *lightsContainer,
                                  GameObject *lightReceiver,
                                  Camera *camera)
{
    Rect maskRect = lightReceiver->GetBoundingScreenRect(camera, true);
    maskRect = Rect::ScreenRectNDC; // TAKE THIS OUT !!!!
    ENSURE(maskRect != Rect::Zero);
    ApplyDeferredLightsToGBuffer(lightsContainer, camera, maskRect);
}

void GEngine::ApplyDeferredLightsToGBuffer(GameObject *lightsContainer,
                                           Camera *camera,
                                           const Rect &maskRectNDC)
{
    // We have marked from before the zone where we want to apply the effect
    GL::SetStencilValue(1);
    GL::Function latestStencilFunc = GL::GetStencilFunc();
    GL::SetStencilFunc(GL::Function::Equal);

    List<Light*> lights = lightsContainer->GetComponentsInChildren<Light>();
    for (Light *light : lights)
    {
        if (!light || !light->IsEnabled(true)) { continue; }
        light->ApplyLight(camera->GetGBuffer(), maskRectNDC);
    }
    GL::SetStencilFunc(latestStencilFunc);
}

void GEngine::Resize(int newWidth, int newHeight)
{
}

void GEngine::_BindCamera(Camera *cam)
{
    ENSURE(cam != p_boundCamera);
    if (p_boundCamera) { p_boundCamera->UnBind(); }

    p_boundCamera = cam;
    if (p_boundCamera) { p_boundCamera->Bind(); }
}

void GEngine::BindCamera(Camera *cam)
{
    GEngine::GetActive()->_BindCamera(cam);
}

GBuffer *GEngine::GetCurrentGBuffer()
{
    Camera *cam = GEngine::GetBoundCamera();
    return cam ? cam->GetGBuffer() : nullptr;
}

Camera *GEngine::GetBoundCamera()
{
    return GEngine::GetActive()->p_boundCamera;
}

SelectionFramebuffer *GEngine::GetCurrentSelectionFramebuffer()
{
    Camera *cam = GEngine::GetBoundCamera();
    return cam ? cam->GetSelectionFramebuffer() : nullptr;
}

void GEngine::RenderToGBuffer(GameObject *go, Camera *camera)
{
    camera->BindGBuffer();

    // GBuffer Scene rendering
    GL::SetDepthMask(true); // Write depth
    GL::SetDepthFunc(GL::Function::LEqual);
    GL::SetStencilValue(1);
    GL::SetStencilOp(GL::StencilOperation::Replace); // Write to stencil
    go->Render(RenderPass::Scene_Lighted);
    GL::SetStencilOp(GL::StencilOperation::Keep); // Dont modify stencil
    ApplyDeferredLights(go, go, camera);
    GL::SetStencilValue(0);

    go->Render(RenderPass::Scene_UnLighted);
    go->Render(RenderPass::Scene_PostProcess);


    // GBuffer Canvas rendering
    camera->GetGBuffer()->SetColorDrawBuffer();
    GL::ClearDepthBuffer();
    GL::ClearStencilBuffer();
    GL::SetDepthMask(true);
    GL::SetDepthFunc(GL::Function::LEqual);
    go->Render(RenderPass::Canvas);
    go->Render(RenderPass::Canvas_PostProcess);

    // GBuffer Gizmos rendering
    GL::ClearDepthBuffer();
    GL::SetDepthMask(true);
    GL::ClearStencilBuffer();
    GL::SetDepthFunc(GL::Function::LEqual);
    go->RenderGizmos();
}

void GEngine::RenderToSelectionFramebuffer(GameObject *go, Camera *camera)
{
    camera->BindSelectionFramebuffer();

    // Selection rendering
    camera->GetSelectionFramebuffer()->PrepareForRender(go);
    go->Render(RenderPass::Scene_Lighted);
    go->Render(RenderPass::Scene_UnLighted);
    GL::ClearStencilBuffer();
    go->Render(RenderPass::Canvas);
    go->RenderGizmos();
}

void GEngine::SetActive(GEngine *gEngine)
{
    GEngine::s_gEngine = gEngine;
    GL::SetActive( gEngine ? gEngine->GetGL() : nullptr );
}


void GEngine::ApplyScreenPass(ShaderProgram *sp, const Rect &mask)
{
    sp->Bind();
    sp->Set("B_rectMinCoord", mask.GetMin());
    sp->Set("B_rectMaxCoord", mask.GetMax());
    RenderScreenPlane();
    sp->UnBind();
}

void GEngine::RenderToScreen(Camera *cam)
{
    ENSURE(cam);
    m_renderGBufferToScreenMaterial->Bind();

    ShaderProgram *sp = m_renderGBufferToScreenMaterial->GetShaderProgram();
    GBuffer *gbuffer = cam->GetGBuffer();
    gbuffer->PrepareForRender(sp);
    sp->Set("B_GTex_Color", gbuffer->GetAttachmentTexture(GBuffer::AttColor));
    if (Input::GetKey(Key::S)) { sp->Set("B_GTex_Color",
                                         cam->GetSelectionFramebuffer()->
                                         GetAttachmentTexture(
                                             SelectionFramebuffer::AttColor)); }

    GEngine::RenderScreenPlane(true);

    m_renderGBufferToScreenMaterial->UnBind();
}

void GEngine::RenderToScreen(Texture *fullScreenTexture)
{
    ASSERT(fullScreenTexture);
    m_renderGBufferToScreenMaterial->Bind();

    ShaderProgram *sp = m_renderGBufferToScreenMaterial->GetShaderProgram();
    sp->Set("B_GTex_Color", fullScreenTexture);

    GEngine::RenderScreenPlane();

    m_renderGBufferToScreenMaterial->UnBind();
}

void GEngine::RenderScreenPlane(bool withDepth)
{
    bool prevWireframe = GL::IsWireframe();
    GL::SetWireframe(false);

    GL::SetViewProjMode(GL::ViewProjMode::IgnoreBothAndModel);

    GL::Face prevCullFace = GL::GetCullFace();
    bool prevDepthMask = GL::GetDepthMask();
    GL::Function prevDepthFunc = GL::GetDepthFunc();

    if (!withDepth)
    {
        GL::SetDepthFunc(GL::Function::Always);
        GL::SetDepthMask(false);
    }
    GL::SetCullFace(GL::Face::None);

    GL::Render(m_screenPlaneMesh->GetVAO(), GL::Primitives::Triangles,
               m_screenPlaneMesh->GetVertexCount());

    GL::SetCullFace(prevCullFace);
    GL::SetDepthMask(prevDepthMask);
    GL::SetDepthFunc(prevDepthFunc);
    GL::SetWireframe(prevWireframe);
}

GEngine* GEngine::GetActive()
{
    Window *win = Window::GetActive();
    return win ? win->GetGEngine() : nullptr;
}

void GEngine::Render(Renderer *rend)
{
    Camera *boundCamera = p_boundCamera; ENSURE(boundCamera);

    if (GL::IsBound(boundCamera->GetGBuffer()))
    {
        rend->Bind();

        if (rend->NeedsReadingColorBuffer())
        {
            boundCamera->GetGBuffer()->PrepareColorReadBuffer(
                                        rend->GetBoundingRect(p_boundCamera));
        }
        Material *rendMat = rend->GetMaterial();
        boundCamera->GetGBuffer()->PrepareForRender(rendMat->GetShaderProgram());
        rend->OnRender();
        rend->UnBind();
    }
    else if (GL::IsBound(boundCamera->GetSelectionFramebuffer()))
    {
        boundCamera->GetSelectionFramebuffer()->RenderForSelectionBuffer(rend);
    }
    else { ASSERT(false); }
}

GL *GEngine::GetGL() const { return m_gl; }
TextureUnitManager *GEngine::GetTextureUnitManager() const
{
    return m_texUnitManager;
}
