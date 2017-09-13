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
#include "Bang/Application.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/RectTransform.h"
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

void GEngine::RenderCurrentScene(Camera *camera)
{
    Texture2D *camRenderTexture = camera->GetRenderTexture();
    if (!camRenderTexture)
    {
        camRenderTexture = Window::GetInstance()->GetScreenRenderTexture();
    }

    camera->Bind();

    GL::SetViewport(0, 0, camRenderTexture->GetWidth(),
                    camRenderTexture->GetHeight());
    m_gbuffer->SetAttachmentTexture(camRenderTexture, GBuffer::AttColor);

    RenderCurrentSceneToGBuffer(camera);
    RenderCurrentSceneToSelectionFramebuffer(camera);
}

void GEngine::Render(Scene *scene)
{
    Scene *prevScene = GEngine::GetCurrentScene();
    GEngine::SetCurrentScene(scene);

    List<Camera*> sceneCameras = scene->GetComponentsInChildren<Camera>();
    for (Camera *cam : sceneCameras)
    {
        RenderCurrentScene(cam);
    }

    GEngine::SetCurrentScene(prevScene);
}

void GEngine::ApplyDeferredLights(Camera *camera, Renderer *rend)
{
    Scene *scene = GEngine::GetCurrentScene();

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

    Material *rendMat = rend ? rend->GetMaterial() : nullptr;
    if ( !rend || (rendMat && rendMat->IsReceivesLighting()) )
    {
        List<Light*> lights = scene->GetComponentsInChildren<Light>();
        for (Light *light : lights)
        {
            if (!light || !light->IsEnabled(true)) { continue; }
            light->ApplyLight(m_gbuffer, renderRect);
        }
    }

    GL::SetStencilFunc(GL::Function::Always);
}

void GEngine::Resize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
    m_selectionFramebuffer->Resize(newWidth, newHeight);
}

void GEngine::SetCurrentScene(Scene *scene)
{
    GEngine::GetInstance()->p_currentScene = scene;
}

Scene *GEngine::GetCurrentScene()
{
    return GEngine::GetInstance()->p_currentScene;
}

GBuffer *GEngine::GetGBuffer()
{
    return GEngine::GetInstance()->m_gbuffer;
}

SelectionFramebuffer *GEngine::GetSelectionFramebuffer()
{
    return GEngine::GetInstance()->m_selectionFramebuffer;
}

void GEngine::RenderCurrentSceneToGBuffer(Camera *camera)
{
    Scene *scene = GEngine::GetCurrentScene();

    m_gbuffer->Bind();
    Color bgColor = camera->GetClearColor();
    m_gbuffer->ClearBuffersAndBackground(bgColor);
    m_gbuffer->SetAllColorDrawBuffers();

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
    m_gbuffer->SetAllColorDrawBuffers();
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

    m_gbuffer->UnBind();
}

void GEngine::RenderCurrentSceneToSelectionFramebuffer(Camera *camera)
{
    Scene *scene = GEngine::GetCurrentScene();

    m_selectionFramebuffer->Bind();
    GL::ClearStencilBuffer();
    m_selectionFramebuffer->ClearDepth();
    m_selectionFramebuffer->ClearColor(Color::One);

    // Selection rendering
    m_selectionFramebuffer->PrepareForRender(scene);
    scene->Render(RenderPass::Scene_Lighted);
    scene->Render(RenderPass::Scene_UnLighted);
    GL::ClearStencilBuffer();
    scene->Render(RenderPass::Canvas);
    scene->RenderGizmos();

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
    Application *app = Application::GetInstance();
    return app ? app->GetGEngine() : nullptr;
}

void GEngine::Render(Renderer *rend)
{
    Scene *scene = GEngine::GetCurrentScene();
    if (GL::IsBound(m_gbuffer))
    {
        rend->Bind();
        Material *rendMat = rend->GetMaterial();

        if (rend->NeedsReadingColorBuffer())
        {
            Camera *cam = scene->GetCamera();
            m_gbuffer->PrepareColorReadBuffer(rend->GetBoundingRect(cam));
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
