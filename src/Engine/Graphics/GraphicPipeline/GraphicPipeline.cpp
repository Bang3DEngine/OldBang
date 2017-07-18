#include "Bang/GraphicPipeline.h"

#include "Bang/Mesh.h"
#include "Bang/Debug.h"
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
#include "Bang/GPPass_RenderLayer.h"
#include "Bang/G_TextureUnitManager.h"
#include "Bang/GraphicPipelineDebugger.h"
#include "Bang/GPPass_SP_DeferredLights.h"
#include "Bang/GPPass_SP_PostProcessEffects.h"

GraphicPipeline::GraphicPipeline(G_Screen *screen)
{
    m_glContext = new GLContext();
    m_texUnitManager = new G_TextureUnitManager();

    m_gbuffer = new G_GBuffer(screen->GetWidth(), screen->GetHeight());

    m_renderGBufferToScreenMaterial =
         AssetsManager::Load<Material>(
                EPATH("Materials/RenderGBufferToScreen.bmat") );

    m_screenPlaneMesh = MeshFactory::GetUIPlane();

    // Set up graphic pipeline passes
    typedef Renderer::RenderLayer RL;
    m_scenePass  =
     new GPPass_RenderLayer(this, RL::Scene,
     {
       new GPPass_G(this, true),     // Lighted G_Pass
       new GPPass_SP_DeferredLights(this),  // Apply light
       new GPPass_G(this, false),    // UnLighted G_Pass
       new GPPass_SP_PostProcessEffects(this,
                                        PostProcessEffect::Type::AfterScene)
     });

    m_canvasPass =
     new GPPass_RenderLayer(this, RL::Canvas,
     {
      new GPPass_G(this, false),
      new GPPass_SP_PostProcessEffects(this,
                                       PostProcessEffect::Type::AfterCanvas)
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
    m_gbuffer->RenderToScreen(m_gbufferAttachToBeShown);
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
    ENSURE(renderRect != Rect::Empty);

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

    m_canvasPass->Pass(renderers, sceneChildren);

    m_gbuffer->UnBind();
}

void GraphicPipeline::ApplyScreenPass(G_ShaderProgram *sp, const Rect &mask)
{
    sp->Bind();
    m_glContext->ApplyToShaderProgram(sp);
    sp->SetVec2("B_rectMinCoord", mask.GetMin());
    sp->SetVec2("B_rectMaxCoord", mask.GetMax());
    sp->SetVec2("B_ScreenSize", Screen::GetSize());
    RenderScreenPlane();
    sp->UnBind();
}

void GraphicPipeline::RenderToScreen(G_Texture *fullScreenTexture)
{
    ShaderProgram *sp = m_renderGBufferToScreenMaterial->GetShaderProgram();
    ENSURE(sp);

    m_renderGBufferToScreenMaterial->Bind();
    m_gbuffer->BindTextureBuffersTo(sp, false);
    GL::ApplyContextToShaderProgram(sp);

    sp->SetTexture("B_GTex_Color", fullScreenTexture);

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

GraphicPipeline* GraphicPipeline::GetActive()
{
    Screen *screen = Screen::GetInstance();
    return screen ? screen->GetGraphicPipeline() : nullptr;
}

void GraphicPipeline::OnResize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
}

void GraphicPipeline::SetGBufferAttachmentToBeRendered(
        G_GBuffer::AttachmentId attachment)
{
    m_gbufferAttachToBeShown = attachment;
}

GLContext *GraphicPipeline::GetGLContext() const
{
    return m_glContext;
}

G_GBuffer *GraphicPipeline::GetGBuffer()
{
    return m_gbuffer;
}

G_TextureUnitManager *GraphicPipeline::GetTextureUnitManager() const
{
    return m_texUnitManager;
}

GraphicPipeline::~GraphicPipeline()
{
    delete m_gbuffer;
    delete m_scenePass;
    delete m_canvasPass;
    delete m_texUnitManager;
    delete m_glContext;
}
