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
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutManager.h"
#include "Bang/MaterialFactory.h"
#include "Bang/TextureUnitManager.h"
#include "Bang/SelectionFramebuffer.h"

USING_NAMESPACE_BANG

GEngine::GEngine()
{
    m_gl = new GL();
    m_texUnitManager = new TextureUnitManager();

    m_gbuffer = new GBuffer(1,1);
    m_selectionFramebuffer = new SelectionFramebuffer(1,1);

    m_renderGBufferToScreenMaterial = MaterialFactory::GetRenderGBufferToScreen();
    m_screenPlaneMesh = MeshFactory::GetUIPlane();
}

GEngine::~GEngine()
{
    delete m_gbuffer;
    delete m_selectionFramebuffer;
    delete m_texUnitManager;
    delete m_gl;
}

void GEngine::Render(GameObject *go, Camera *camera)
{
    Texture2D *camRT = camera->GetRenderTexture();
    if (camRT)
    {
        Vector2i newVpSize = camRT->GetSize();
        Vector2i prevVpSize = GL::GetViewportSize();
        GL::SetViewport(0, 0, newVpSize.x, newVpSize.y);
        if (prevVpSize != newVpSize) { UILayoutManager::RebuildLayout(go); }

        m_gbuffer->Resize(camRT->GetWidth(), camRT->GetHeight());
        m_selectionFramebuffer->Resize(camRT->GetWidth(), camRT->GetHeight());
        m_gbuffer->SetAttachmentTexture(camRT, GBuffer::AttColor);
        // m_selectionFramebuffer->SetAttachmentTexture(camRT, GL::Attachment::Color0);

        RenderToGBuffer(go, camera);
        RenderToSelectionFramebuffer(go, camera);
    }
}

void GEngine::Render(Scene *scene)
{
    Camera *sceneCam = scene->GetCamera();
    if (sceneCam) { Render(scene, sceneCam); }
}

void GEngine::ApplyDeferredLights(GameObject *lightsContainer,
                                  Renderer *lightReceiver,
                                  Camera *camera)
{
    Rect maskRect = lightReceiver->gameObject->GetBoundingScreenRect(camera, false);
    maskRect = Rect::ScreenRect; // TAKE THIS OUT !!!!
    ENSURE(maskRect != Rect::Zero);
    ApplyDeferredLightsToGBuffer(lightsContainer, maskRect);
}

void GEngine::ApplyDeferredLights(GameObject *lightsContainer,
                                  GameObject *lightReceiver,
                                  Camera *camera)
{
    Rect maskRect = lightReceiver->GetBoundingScreenRect(camera, true);
    maskRect = Rect::ScreenRect; // TAKE THIS OUT !!!!
    ENSURE(maskRect != Rect::Zero);
    ApplyDeferredLightsToGBuffer(lightsContainer, maskRect);
}

void GEngine::ApplyDeferredLightsToGBuffer(GameObject *lightsContainer,
                                           const Rect &maskRectNDC)
{
    // We have marked from before the zone where we want to apply the effect
    GL::SetStencilValue(1);
    GL::SetStencilFunc(GL::Function::Equal);

    List<Light*> lights = lightsContainer->GetComponentsInChildren<Light>();
    for (Light *light : lights)
    {
        if (!light || !light->IsEnabled(true)) { continue; }
        light->ApplyLight(m_gbuffer, maskRectNDC);
    }
    GL::SetStencilFunc(GL::Function::Always);
}

void GEngine::Resize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
    m_selectionFramebuffer->Resize(newWidth, newHeight);
}

void GEngine::_BindCamera(Camera *cam)
{
    cam->Bind();
    p_boundCamera = cam;
}

void GEngine::BindCamera(Camera *cam)
{
    GEngine::GetInstance()->_BindCamera(cam);
}

GBuffer *GEngine::GetGBuffer()
{
    return GEngine::GetInstance()->m_gbuffer;
}

Camera *GEngine::GetBoundCamera()
{
    return GEngine::GetInstance()->p_boundCamera;
}

SelectionFramebuffer *GEngine::GetSelectionFramebuffer()
{
    return GEngine::GetInstance()->m_selectionFramebuffer;
}

void GEngine::RenderToGBuffer(GameObject *go, Camera *camera)
{
    m_gbuffer->Bind();
    _BindCamera(camera);

    Color bgColor = camera->GetClearColor();
    m_gbuffer->ClearBuffersAndBackground(bgColor);
    m_gbuffer->SetAllColorDrawBuffers();

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
    m_gbuffer->SetAllColorDrawBuffers();
    GL::ClearDepthBuffer();
    GL::ClearStencilBuffer();
    GL::SetDepthMask(true);
    GL::SetDepthFunc(GL::Function::LEqual);
    go->Render(RenderPass::Canvas);
    go->Render(RenderPass::Canvas_PostProcess);
    GL::ClearStencilBuffer();

    // GBuffer Gizmos rendering
    GL::SetDepthMask(true);
    GL::SetDepthFunc(GL::Function::LEqual);
    go->RenderGizmos();

    m_gbuffer->UnBind();
}

void GEngine::RenderToSelectionFramebuffer(GameObject *go, Camera *camera)
{
    camera->Bind();

    m_selectionFramebuffer->Bind();
    GL::ClearStencilBuffer();
    m_selectionFramebuffer->ClearDepth();
    m_selectionFramebuffer->ClearColor(Color::One);

    // Selection rendering
    m_selectionFramebuffer->PrepareForRender(go);
    go->Render(RenderPass::Scene_Lighted);
    go->Render(RenderPass::Scene_UnLighted);
    GL::ClearStencilBuffer();
    go->Render(RenderPass::Canvas);
    go->RenderGizmos();

    m_selectionFramebuffer->UnBind();
}


void GEngine::ApplyScreenPass(ShaderProgram *sp, const Rect &mask)
{
    sp->Bind();
    m_gl->ApplyToShaderProgram(sp);
    sp->Set("B_rectMinCoord", mask.GetMin());
    sp->Set("B_rectMaxCoord", mask.GetMax());
    sp->Set("B_ScreenSize", Vector2f(GL::GetViewportSize()));
    RenderScreenPlane();
    sp->UnBind();
}

void GEngine::RenderToScreen(Texture *fullScreenTexture)
{
    ASSERT(fullScreenTexture);
    m_renderGBufferToScreenMaterial->Bind();

    ShaderProgram *sp = m_renderGBufferToScreenMaterial->GetShaderProgram();
    m_gbuffer->PrepareForRender(sp);
    sp->Set("B_GTex_Color", fullScreenTexture);

    GEngine::RenderScreenPlane();

    m_renderGBufferToScreenMaterial->UnBind();
}

void GEngine::RenderScreenPlane()
{
    GL::SetWireframe(false);
    GL::SetDepthFunc(GL::Function::Always);
    GL::SetDepthMask(false);
    GL::SetCullFace(GL::Face::None);
    GL::Render(m_screenPlaneMesh->GetVAO(), GL::Primitives::Triangles,
               m_screenPlaneMesh->GetVertexCount());
    GL::SetDepthMask(true);
    GL::SetDepthFunc(GL::Function::LEqual);
}

GEngine* GEngine::GetInstance()
{
    Window *win = Window::GetCurrent();
    return win ? win->GetGEngine() : nullptr;
}

void GEngine::Render(Renderer *rend)
{
    if (GL::IsBound(m_gbuffer))
    {
        rend->Bind();
        Material *rendMat = rend->GetMaterial();

        if (rend->NeedsReadingColorBuffer())
        {
            m_gbuffer->PrepareColorReadBuffer(
                        rend->GetBoundingRect(p_boundCamera));
        }
        m_gbuffer->PrepareForRender(rendMat->GetShaderProgram());
        rend->OnRender();
        rend->UnBind();
    }
    else if (GL::IsBound(m_selectionFramebuffer))
    {
        m_selectionFramebuffer->RenderForSelectionBuffer(rend);
    }
}

GL *GEngine::GetGL() const { return m_gl; }
TextureUnitManager *GEngine::GetTextureUnitManager() const
{
    return m_texUnitManager;
}
