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
#include "Bang/Resources.h"
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

    p_screenPlaneMesh = Resources::Clone<Mesh>(MeshFactory::GetUIPlane());
    p_renderGBufferToScreenMaterial = MaterialFactory::GetRenderGBufferToScreen();
    GL::SetActive( nullptr );
}

void GEngine::Render(GameObject *go, Camera *camera)
{
    go->BeforeRender();
    SetActiveCamera(camera);
    RenderToGBuffer(go, camera);
    RenderToSelectionFramebuffer(go, camera);
    SetActiveCamera(nullptr);
}

void GEngine::ApplyStenciledDeferredLightsToGBuffer(GameObject *lightsContainer,
                                                    Camera *camera,
                                                    const Rect &maskRectNDC)
{
    Byte prevStencilValue              = GL::GetStencilValue();
    GL::Function prevStencilFunc       = GL::GetStencilFunc();
    GL::StencilOperation prevStencilOp = GL::GetStencilOp();

    // We have marked from before the zone where we want to apply the effect
    GL::SetStencilOp(GL::StencilOperation::Keep);
    GL::SetStencilFunc(GL::Function::Equal);
    GL::SetStencilValue(1);

    List<Light*> lights = lightsContainer->GetComponentsInChildren<Light>();
    for (Light *light : lights)
    {
        if (!light || !light->IsEnabled(true)) { continue; }
        light->ApplyLight(camera, maskRectNDC);
    }

    GL::SetStencilValue(prevStencilValue);
    GL::SetStencilFunc(prevStencilFunc);
    GL::SetStencilOp(prevStencilOp);
}

void GEngine::Resize(int newWidth, int newHeight)
{
}

Camera *GEngine::GetActiveCamera()
{
    GEngine *ge = GEngine::GetActive();
    return ge ? ge->p_activeCamera : nullptr;
}

GBuffer *GEngine::GetActiveGBuffer()
{
    Camera *cam = GEngine::GetActiveCamera();
    return cam ? cam->GetGBuffer() : nullptr;
}

SelectionFramebuffer *GEngine::GetActiveSelectionFramebuffer()
{
    Camera *cam = GEngine::GetActiveCamera();
    return cam ? cam->GetSelectionFramebuffer() : nullptr;
}

void GEngine::RenderToGBuffer(GameObject *go, Camera *camera)
{
    camera->BindGBuffer();

    // GBuffer Scene rendering
    camera->GetGBuffer()->SetAllDrawBuffers();
    GL::SetDepthMask(true);
    GL::SetDepthFunc(GL::Function::LEqual);
    RenderWithPassAndMarkStencilForLights(go, RenderPass::Scene);
    ApplyStenciledDeferredLightsToGBuffer(go, camera);
    camera->GetGBuffer()->SetColorDrawBuffer();
    RenderWithPass(go, RenderPass::ScenePostProcess);

    // GBuffer Canvas rendering
    camera->GetGBuffer()->SetColorDrawBuffer();
    GL::Enable(GL::Test::Blend);
    GL::BlendFunc(GL::BlendFactor::SrcAlpha, GL::BlendFactor::OneMinusSrcAlpha);
    GL::ClearStencilBuffer();
    GL::ClearDepthBuffer();
    GL::SetDepthMask(true);
    GL::SetDepthFunc(GL::Function::LEqual);
    RenderWithPass(go, RenderPass::Canvas);
    RenderWithPass(go, RenderPass::CanvasPostProcess);
    GL::Disable(GL::Test::Blend);

    // GBuffer Overlay rendering
    camera->GetGBuffer()->SetAllDrawBuffers();
    GL::ClearStencilBuffer();
    GL::ClearDepthBuffer();
    GL::SetDepthMask(true);
    GL::SetDepthFunc(GL::Function::LEqual);
    RenderWithPassAndMarkStencilForLights(go, RenderPass::Overlay);
    ApplyStenciledDeferredLightsToGBuffer(go, camera);
    RenderWithPass(go, RenderPass::OverlayPostProcess);
}

void GEngine::RenderToSelectionFramebuffer(GameObject *go, Camera *camera)
{
    if (camera->GetRenderSelectionBuffer())
    {
        camera->BindSelectionFramebuffer();

        // Selection rendering
        camera->GetSelectionFramebuffer()->PrepareNewFrameForRender(go);
        go->Render(RenderPass::Scene);
        GL::ClearStencilBuffer();
        GL::ClearDepthBuffer();
        RenderWithPass(go, RenderPass::Canvas);

        RenderWithPass(go, RenderPass::Overlay);
    }
}

void GEngine::RenderWithPass(GameObject *go, RenderPass renderPass)
{
    Camera *cam = GetActiveCamera();
    if (cam && cam->MustRenderPass(renderPass))
    {
        go->Render(renderPass, true);
    }
}

void GEngine::RenderWithPassAndMarkStencilForLights(GameObject *go,
                                                    RenderPass renderPass)
{
    Byte prevStencilValue                     = GL::GetStencilValue();
    GL::StencilOperation prevStencilOperation = GL::GetStencilOp();

    GL::SetStencilValue(1);
    GL::SetStencilOp(GL::StencilOperation::Replace);

    // Render pass
    RenderWithPass(go, renderPass);

    // Restore
    GL::SetStencilOp(prevStencilOperation);
    GL::SetStencilValue(prevStencilValue);
}

void GEngine::SetActive(GEngine *gEngine)
{
    GEngine::s_gEngine = gEngine;
    GL::SetActive( gEngine ? gEngine->GetGL() : nullptr );
}


void GEngine::RenderScreenRect(ShaderProgram *sp, const Rect &destRectMask)
{
    GLId prevBoundShaderProgram = GL::GetBoundId(GL::BindTarget::ShaderProgram);
    sp->Bind();
    sp->Set("B_UvOffset",         Vector2::Zero, false);
    sp->Set("B_UvMultiply",       Vector2::One, false);
    sp->Set("B_destRectMinCoord", destRectMask.GetMin(), false);
    sp->Set("B_destRectMaxCoord", destRectMask.GetMax(), false);
    RenderScreenPlane();
    sp->UnBind();
    GL::Bind(GL::BindTarget::ShaderProgram, prevBoundShaderProgram);
}

void GEngine::RenderGBufferToScreen(const Rect &gbufferRectMask,
                                    const Rect &destRectMask)
{
    GBuffer *gbuffer = Camera::GetActive()->GetGBuffer();
    GLId prevBoundShaderProgram = GL::GetBoundId(GL::BindTarget::ShaderProgram);

    ShaderProgram *sp = p_renderGBufferToScreenMaterial.Get()->GetShaderProgram();

    sp->Bind();
    gbuffer->PrepareForRender(sp);
    sp->Set("B_GTex_Color", gbuffer->GetAttachmentTexture(GBuffer::AttColor),
            false);

    sp->Set("B_UvOffset",         gbufferRectMask.GetMin()  * 0.5f + 0.5f);
    sp->Set("B_UvMultiply",       gbufferRectMask.GetSize() * 0.5f);
    sp->Set("B_destRectMinCoord", destRectMask.GetMin());
    sp->Set("B_destRectMaxCoord", destRectMask.GetMax());
    RenderScreenPlane();
    sp->UnBind();

    GL::Bind(GL::BindTarget::ShaderProgram, prevBoundShaderProgram);
}

void GEngine::RenderToScreen(Camera *cam)
{
    if (!cam) { return; }
    p_renderGBufferToScreenMaterial.Get()->Bind();

    ShaderProgram *sp = p_renderGBufferToScreenMaterial.Get()->GetShaderProgram();
    GBuffer *gbuffer = cam->GetGBuffer();
    gbuffer->PrepareForRender(sp);
    sp->Set("B_GTex_Color", gbuffer->GetAttachmentTexture(GBuffer::AttColor),
            false);

    GEngine::RenderScreenRect(sp, Rect::NDCRect);

    p_renderGBufferToScreenMaterial.Get()->UnBind();
}

void GEngine::RenderToScreen(Texture2D *fullScreenTexture)
{
    ASSERT(fullScreenTexture);
    p_renderGBufferToScreenMaterial.Get()->Bind();

    ShaderProgram *sp = p_renderGBufferToScreenMaterial.Get()-> GetShaderProgram();
    sp->Set("B_GTex_Color", fullScreenTexture, false);

    GEngine::RenderScreenPlane();

    p_renderGBufferToScreenMaterial.Get()->UnBind();
}

void GEngine::RenderScreenPlane(bool withDepth)
{
    bool prevWireframe = GL::IsWireframe();
    GL::ViewProjMode prevViewProjMode = GL::GetViewProjMode();
    bool prevDepthMask = GL::GetDepthMask();

    GL::SetWireframe(false);
    GL::SetViewProjMode(GL::ViewProjMode::IgnoreBothAndModel);
    GL::Function prevDepthFunc = GL::GetDepthFunc();

    if (!withDepth)
    {
        GL::SetDepthFunc(GL::Function::Always);
        GL::SetDepthMask(false);
    }

    GL::Render(p_screenPlaneMesh.Get()->GetVAO(), GL::Primitive::Triangles,
               p_screenPlaneMesh.Get()->GetVertexCount());

    GL::SetDepthMask(prevDepthMask);
    GL::SetDepthFunc(prevDepthFunc);
    GL::SetWireframe(prevWireframe);
    GL::SetViewProjMode(prevViewProjMode);
}

GEngine* GEngine::GetActive()
{
    Window *win = Window::GetActive();
    return win ? win->GetGEngine() : nullptr;
}

void GEngine::SetActiveCamera(Camera *camera)
{
    if (p_activeCamera) { p_activeCamera->UnBind(); }

    p_activeCamera = camera;
    if (p_activeCamera) { p_activeCamera->Bind(); }
}

void GEngine::Render(Renderer *rend)
{
    Camera *activeCamera = GetActiveCamera();
    if (!activeCamera) { return; }

    if (GL::IsBound(activeCamera->GetSelectionFramebuffer()))
    {
        activeCamera->GetSelectionFramebuffer()->RenderForSelectionBuffer(rend);
    }
    else
    {
        ASSERT( GL::IsBound(activeCamera->GetGBuffer()) ||
                GL::GetBoundId(GL::BindTarget::DrawFramebuffer) > 0 );

        rend->Bind();

        Material *rendMat = rend->GetUserMaterial();
        if (rendMat)
        {
            activeCamera->GetGBuffer()->PrepareForRender(rendMat->GetShaderProgram());
            rend->OnRender();
            rend->UnBind();
        }
    }
}

GL *GEngine::GetGL() const { return m_gl; }
TextureUnitManager *GEngine::GetTextureUnitManager() const
{
    return m_texUnitManager;
}
