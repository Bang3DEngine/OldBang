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
#include "Bang/Texture.h"
#include "Bang/ChronoGL.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/Application.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/RectTransform.h"
#include "Bang/RenderSurface.h"
#include "Bang/MaterialFactory.h"
#include "Bang/TextureUnitManager.h"
#include "Bang/SelectionFramebuffer.h"

USING_NAMESPACE_BANG

GEngine::GEngine()
{
    m_gl = new GL();
    m_texUnitManager = new TextureUnitManager();

    m_renderGBufferToScreenMaterial = MaterialFactory::GetRenderGBufferToScreen();
    m_screenPlaneMesh = MeshFactory::GetUIPlane();
}

GEngine::~GEngine()
{
    delete m_texUnitManager;
    delete m_gl;
}

void GEngine::RenderCurrentScene(Camera *_camera)
{
    Scene *scene = GEngine::GetCurrentScene();

    Camera *camera = _camera;
    if (!camera) { camera = scene->GetCamera(); }
    if (camera) { camera->Bind(); }

    RenderSurface *currentRF = GEngine::GetCurrentRenderSurface();
    if (!currentRF) { currentRF = Window::GetInstance()->GetRenderSurface(); }
    GEngine::SetCurrentRenderSurface(currentRF);

    GBuffer *gbuffer = GEngine::GetCurrentGBuffer();
    RenderCurrentSceneToGBuffer(camera);
    RenderToScreen(gbuffer->GetAttachmentTexture(GBuffer::AttColor));

    SelectionFramebuffer *selFB = GEngine::GetCurrentSelectionFramebuffer();
    RenderCurrentSceneToSelectionFramebuffer(camera);
    // RenderToScreen(selFB->GetColorTexture());
}

void GEngine::Render(Scene *scene, Camera *camera)
{
    GEngine::SetCurrentScene(scene);
    RenderCurrentScene(camera);
}

void GEngine::ApplyDeferredLights(Camera *camera, Renderer *rend)
{
    Scene *scene = GEngine::GetCurrentScene();
    GBuffer *gbuffer = GEngine::GetCurrentGBuffer();

    // Limit rendering to the renderer visible rect
    Rect renderRect = Rect::ScreenRect;
    if (rend)
    {
        renderRect = rend->gameObject->GetBoundingScreenRect(camera, false);
    }
    else
    {
        // Apply deferred lights to the whole scene
        renderRect = scene->GetBoundingScreenRect(camera, true);
    }
    renderRect = Rect::ScreenRect;
    ENSURE(renderRect != Rect::Zero);

    // We have marked from before the zone where we want to apply the effect
    GL::SetStencilValue(1);
    GL::SetStencilFunc(GL::Function::Equal);

    // m_gbuffer->ExportStencil(Path("stencil.png"), 64);
    Material *rendMat = rend ? rend->GetMaterial() : nullptr;
    if ( !rend || (rendMat && rendMat->IsReceivesLighting()) )
    {
        List<Light*> lights = scene->GetComponentsInChildren<Light>();
        for (Light *light : lights)
        {
            if (!light || !light->IsEnabled(true)) { continue; }
            light->ApplyLight(gbuffer, renderRect);
        }
    }

    GL::SetStencilFunc(GL::Function::Always);
}

void GEngine::SetCurrentScene(Scene *scene)
{
    GEngine::GetInstance()->p_currentScene = scene;
}

void GEngine::SetCurrentRenderSurface(RenderSurface *renderSurface)
{
    GEngine::GetInstance()->p_currentRenderSurface = renderSurface;
}

Scene *GEngine::GetCurrentScene()
{
    return GEngine::GetInstance()->p_currentScene;
}

GBuffer *GEngine::GetCurrentGBuffer()
{
    return GEngine::GetCurrentRenderSurface()->GetGBuffer();
}

SelectionFramebuffer *GEngine::GetCurrentSelectionFramebuffer()
{
    return GEngine::GetCurrentRenderSurface()->GetSelectionFramebuffer();
}

RenderSurface *GEngine::GetCurrentRenderSurface()
{
    return GEngine::GetInstance()->p_currentRenderSurface;
}

void GEngine::RenderCurrentSceneToGBuffer(Camera *camera)
{
    Scene *scene = GEngine::GetCurrentScene();
    GBuffer *gbuffer = GEngine::GetCurrentGBuffer();

    gbuffer->Bind();
    Color bgColor = camera->GetClearColor();
    gbuffer->ClearBuffersAndBackground(bgColor);
    gbuffer->SetAllColorDrawBuffers();

    // GBuffer Scene rendering
    GL::SetDepthMask(true); // Write depth
    GL::SetDepthFunc(GL::Function::LEqual);

    GL::SetStencilValue(1);
    GL::SetStencilOp(GL::StencilOperation::Replace); // Write to stencil
    scene->Render(RenderPass::Scene_Lighted);
    GL::SetStencilOp(GL::StencilOperation::Keep); // Dont modify stencil
    ApplyDeferredLights(camera);

    scene->Render(RenderPass::Scene_UnLighted);
    scene->Render(RenderPass::Scene_PostProcess);
    GL::ClearDepthBuffer();
    GL::ClearStencilBuffer();

    // GBuffer Canvas rendering
    gbuffer->SetAllColorDrawBuffers();
    GL::SetDepthMask(false);
    GL::SetDepthFunc(GL::Function::Always);
    scene->Render(RenderPass::Canvas);
    scene->Render(RenderPass::Canvas_PostProcess);
    GL::SetDepthFunc(GL::Function::LEqual);
    GL::ClearStencilBuffer();

    // GBuffer Gizmos rendering
    GL::SetDepthMask(true);
    GL::SetDepthFunc(GL::Function::LEqual);
    scene->RenderGizmos();

    gbuffer->UnBind();
}

void GEngine::RenderCurrentSceneToSelectionFramebuffer(Camera *camera)
{
    Scene *scene = GEngine::GetCurrentScene();
    SelectionFramebuffer *selFB = GEngine::GetCurrentSelectionFramebuffer();

    selFB->Bind();
    GL::ClearStencilBuffer();
    selFB->ClearDepth();
    selFB->ClearColor(Color::One);

    // Selection rendering
    selFB->PrepareForRender(scene);
    scene->Render(RenderPass::Scene_Lighted);
    scene->Render(RenderPass::Scene_UnLighted);
    GL::ClearStencilBuffer();
    scene->Render(RenderPass::Canvas);
    scene->RenderGizmos();

    selFB->UnBind();
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
    m_renderGBufferToScreenMaterial->Bind();

    ShaderProgram *sp = m_renderGBufferToScreenMaterial->GetShaderProgram();
    // gbuffer->PrepareForRender(sp);
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
    Application *app = Application::GetInstance();
    return app ? app->GetGEngine() : nullptr;
}

void GEngine::Render(Renderer *rend)
{
    Scene *scene = GEngine::GetCurrentScene();
    GBuffer *gbuffer = GEngine::GetCurrentGBuffer();
    SelectionFramebuffer *selFB = GEngine::GetCurrentSelectionFramebuffer();
    if (GL::IsBound(gbuffer))
    {
        rend->Bind();
        Material *rendMat = rend->GetMaterial();

        if (rend->NeedsReadingColorBuffer())
        {
            Camera *cam = scene->GetCamera();
            gbuffer->PrepareColorReadBuffer(rend->GetBoundingRect(cam));
        }
        gbuffer->PrepareForRender(rendMat->GetShaderProgram());
        rend->OnRender();
        rend->UnBind();
    }
    else if (GL::IsBound(selFB))
    {
        selFB->RenderForSelectionBuffer(rend);
    }
}

GL *GEngine::GetGL() const { return m_gl; }
TextureUnitManager *GEngine::GetTextureUnitManager() const
{
    return m_texUnitManager;
}
