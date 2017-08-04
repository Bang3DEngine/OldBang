#include "Bang/GraphicPipeline.h"

#include "Bang/Mesh.h"
#include "Bang/G_VAO.h"
#include "Bang/G_VBO.h"
#include "Bang/Scene.h"
#include "Bang/Light.h"
#include "Bang/Input.h"
#include "Bang/Screen.h"
#include "Bang/Camera.h"
#include "Bang/ChronoGL.h"
#include "Bang/Material.h"
#include "Bang/GPPass_G.h"
#include "Bang/Renderer.h"
#include "Bang/Transform.h"
#include "Bang/G_GBuffer.h"
#include "Bang/G_Texture.h"
#include "Bang/GLContext.h"
#include "Bang/GameObject.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/RectTransform.h"
#include "Bang/G_RenderTexture.h"
#include "Bang/GPPass_G_Gizmos.h"
#include "Bang/GPPass_Selection.h"
#include "Bang/GPPass_RenderLayer.h"
#include "Bang/G_TextureUnitManager.h"
#include "Bang/SelectionFramebuffer.h"
#include "Bang/GPPass_SP_DeferredLights.h"
#include "Bang/GPPass_SP_PostProcessEffects.h"

GraphicPipeline::GraphicPipeline(G_Screen *screen)
{
    m_glContext = new GLContext();
    m_texUnitManager = new G_TextureUnitManager();

    m_gbuffer = new G_GBuffer(screen->GetWidth(),
                              screen->GetHeight());
    m_selectionFB = new SelectionFramebuffer(screen->GetWidth(),
                                             screen->GetHeight());

    m_renderGBufferToScreenMaterial =
         AssetsManager::Load<Material>(
                EPATH("Materials/RenderGBufferToScreen.bmat") );

    m_screenPlaneMesh = MeshFactory::GetUIPlane();

    // Set up graphic pipeline passes
    using RL = Renderer::RenderLayer;
    m_scenePass  =
     new GPPass_RenderLayer(this, RL::RLScene,
     {
       new GPPass_G(this, GPPass_G::ReceiveLightPass::Yes), // Lighted G_Pass
       new GPPass_SP_DeferredLights(this),                  // Apply light
       new GPPass_G(this, GPPass_G::ReceiveLightPass::No),  // UnLighted G_Pass
       new GPPass_SP_PostProcessEffects(this,
                                        PostProcessEffect::Type::AfterScene)
     });

    m_canvasPass =
     new GPPass_RenderLayer(this, RL::RLCanvas,
     {
      new GPPass_G(this, GPPass_G::ReceiveLightPass::Both),
      new GPPass_SP_PostProcessEffects(this,
                                       PostProcessEffect::Type::AfterCanvas)
     });

    m_gizmosPass = new GPPass_RenderLayer(this, RL::RLGizmos,
    {
     new GPPass_G_Gizmos(this)
    });

    m_sceneSelectionPass = new GPPass_RenderLayer(this, RL::RLScene,
    {
      new GPPass_Selection(this)
    });
    m_canvasSelectionPass = new GPPass_RenderLayer(this, RL::RLCanvas,
    {
      new GPPass_Selection(this)
    });
}

void GraphicPipeline::RenderScene(Scene *scene, bool inGame)
{
    p_scene = scene; ENSURE(p_scene);
    m_renderingInGame = inGame;

    List<Renderer*> renderers = scene->GetComponentsInChildren<Renderer>();
    List<GameObject*> sceneChildren = scene->GetChildren();

    Camera *camera = scene->GetCamera();
    if (camera) { camera->Bind(); }

    RenderGBuffer(renderers, sceneChildren);
    RenderToScreen(m_gbuffer->GetAttachmentTexture(G_GBuffer::AttColor));

    RenderSelectionBuffer(renderers, sceneChildren, scene);
    // RenderToScreen(m_selectionFB->GetColorTexture());
}

void GraphicPipeline::ApplyDeferredLights(Renderer *rend)
{
    // Limit rendering to the renderer visible rect
    Rect renderRect = Rect::ScreenRect;
    Camera *sceneCam = p_scene->GetCamera();
    if (rend)
    {
        renderRect = rend->gameObject->GetBoundingScreenRect(sceneCam, false);
    }
    else
    {
        // Apply deferred lights to the whole scene
        renderRect = p_scene->GetBoundingScreenRect(sceneCam, true);
        // renderRect = Rect::ScreenRect;
    }
    ENSURE(renderRect != Rect::Zero);

    // We have marked from before the zone where we want to apply the effect
    m_gbuffer->SetStencilTest(true);

    Material *rendMat = rend ? rend->GetMaterial() : nullptr;
    if ( !rend || (rendMat && rendMat->ReceivesLighting()) )
    {
        List<Light*> lights = p_scene->GetComponentsInChildren<Light>();
        for (Light *light : lights)
        {
            if (!light || !light->IsEnabled()) { continue; }
            light->ApplyLight(m_gbuffer, renderRect);
        }
    }
}

void GraphicPipeline::RenderGBuffer(const List<Renderer*> &renderers,
                                    const List<GameObject*> &sceneChildren)
{
    m_gbuffer->Bind();

    Color bgColor = p_scene->GetCamera()->GetClearColor();
    m_gbuffer->ClearBuffersAndBackground(bgColor);

    m_scenePass->Pass(renderers, sceneChildren);
    m_gbuffer->ClearStencil();

    GL::SetTestDepth(false);
    m_canvasPass->Pass(renderers, sceneChildren);
    GL::SetTestDepth(true);

    m_gizmosPass->Pass(renderers, sceneChildren);

    m_gbuffer->UnBind();
}

void GraphicPipeline::ApplyScreenPass(G_ShaderProgram *sp, const Rect &mask)
{
    sp->Bind();
    m_glContext->ApplyToShaderProgram(sp);
    sp->Set("B_rectMinCoord", mask.GetMin());
    sp->Set("B_rectMaxCoord", mask.GetMax());
    sp->Set("B_ScreenSize", Vector2f(Screen::GetSize()));
    RenderScreenPlane();
    sp->UnBind();
}

void GraphicPipeline::RenderToScreen(G_Texture *fullScreenTexture)
{
    m_renderGBufferToScreenMaterial->Bind();

    ShaderProgram *sp = m_renderGBufferToScreenMaterial->GetShaderProgram();
    m_gbuffer->BindTextureBuffersTo(sp, false);
    sp->Set("B_GTex_Color", fullScreenTexture);

    GraphicPipeline::RenderScreenPlane();

    m_renderGBufferToScreenMaterial->UnBind();
}

void GraphicPipeline::RenderScreenPlane()
{
    GL::SetWireframe(false);
    GL::SetTestDepth(false);
    GL::SetWriteDepth(false);
    GL::SetCullMode(GL::CullMode::None);
    GL::Render(m_screenPlaneMesh->GetVAO(), GL::RenderMode::Triangles,
               m_screenPlaneMesh->GetVertexCount());
    GL::SetWriteDepth(true);
    GL::SetTestDepth(true);
}

void GraphicPipeline::Render(Renderer *renderer) const
{
    ENSURE(renderer && renderer->IsEnabled());
    renderer->Bind();
    m_gbuffer->BindTextureBuffersTo(
                renderer->GetMaterial()->GetShaderProgram(), false);
    renderer->Render();
    renderer->UnBind();
}

void GraphicPipeline::RenderSelectionBuffer(
                        const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren,
                        Scene *scene)
{
    m_selectionFB->Bind();

    m_selectionFB->ClearDepth();
    m_selectionFB->ClearColor(Color::One);

    m_selectionFB->PrepareForRender(scene);
    m_sceneSelectionPass->Pass (renderers, sceneChildren);
    m_canvasSelectionPass->Pass(renderers, sceneChildren);

    m_selectionFB->UnBind();
}

GraphicPipeline* GraphicPipeline::GetActive()
{
    Screen *screen = Screen::GetInstance();
    return screen ? screen->GetGraphicPipeline() : nullptr;
}

void GraphicPipeline::OnResize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
    m_selectionFB->Resize(newWidth, newHeight);
}

GLContext *GraphicPipeline::GetGLContext() const
{
    return m_glContext;
}

G_GBuffer *GraphicPipeline::GetGBuffer()
{
    return m_gbuffer;
}

SelectionFramebuffer *GraphicPipeline::GetSelectionFramebuffer()
{
    return m_selectionFB;
}

G_TextureUnitManager *GraphicPipeline::GetTextureUnitManager() const
{
    return m_texUnitManager;
}

GraphicPipeline::~GraphicPipeline()
{
    delete m_gbuffer;
    delete m_selectionFB;
    delete m_scenePass;
    delete m_canvasPass;
    delete m_gizmosPass;
    delete m_texUnitManager;
    delete m_glContext;
}
