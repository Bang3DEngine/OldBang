#include "Bang/GraphicPipeline.h"

#include "Bang/VAO.h"
#include "Bang/VBO.h"
#include "Bang/Mesh.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Light.h"
#include "Bang/Input.h"
#include "Bang/Screen.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/Texture.h"
#include "Bang/ChronoGL.h"
#include "Bang/Material.h"
#include "Bang/GPPass_G.h"
#include "Bang/Transform.h"
#include "Bang/GLContext.h"
#include "Bang/GameObject.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/RenderTexture.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"
#include "Bang/RectTransform.h"
#include "Bang/GPPass_SP_FXAA.h"
#include "Bang/GPPass_G_Gizmos.h"
#include "Bang/TextureUnitManager.h"
#include "Bang/GraphicPipelineDebugger.h"
#include "Bang/GPPass_SP_DeferredLights.h"

#ifdef BANG_EDITOR
#include "Bang/Hierarchy.h"
#include "Bang/GPPass_Selection.h"
#include "Bang/SelectionFramebuffer.h"
#endif

GraphicPipeline::GraphicPipeline(Screen *screen)
{
    m_glContext = new GLContext();
    m_texUnitManager = new TextureUnitManager();

    m_gbuffer = new GBuffer(screen->m_width, screen->m_height);
    #ifdef BANG_EDITOR
    m_selectionFB = new SelectionFramebuffer(screen->m_width, screen->m_height);
    #endif


    m_matSelectionEffectScreen = AssetsManager::Load<Material>(
                "Materials/SP_SelectionEffect.bmat", true);

    m_renderGBufferToScreenMaterial =
         AssetsManager::Load<Material>("Materials/RenderGBufferToScreen.bmat",
                                       true);

    m_screenPlaneMesh = MeshFactory::GetUIPlane();

    // Set up graphic pipeline passes
    typedef Renderer::DepthLayer DL;
    m_scenePass  =
     new GPPass_DepthLayer(this, DL::DepthLayerScene,
     {
       new GPPass_G(this, true, false),     // Lighted opaques G Pass
       new GPPass_SP_DeferredLights(this),  // Apply light to opaques
       new GPPass_G(this, false, false),    // UnLighted opaques G Pass

       new GPPass_G(this, true, true,       // Lighted Transparent G pass
       {
         new GPPass_SP_DeferredLights(this) //   Add light to transparent
       }),
       new GPPass_G(this, false, true)     // Unlighted Transparent
     });

    m_canvasPass =
     new GPPass_DepthLayer(this, DL::DepthLayerCanvas,
     {
      new GPPass_G(this, false, false),  // Canvas opaques
      new GPPass_G(this, false, true)    // Canvas transparents
     });

    m_gizmosPass = new GPPass_DepthLayer(this, DL::DepthLayerGizmos,
    {
     new GPPass_G_Gizmos(this, true,  false), // Gizmos with depth
     new GPPass_G_Gizmos(this, false, false), // Gizmos normal
     new GPPass_G_Gizmos(this, false,  true)  // Gizmos with overlay
    });

    m_fxaaPass = new GPPass_SP_FXAA(this);

    #ifdef BANG_EDITOR
    m_sceneSelectionPass  = new GPPass_DepthLayer(this, DL::DepthLayerScene,
    {
      new GPPass_Selection(this)
    });
    m_canvasSelectionPass  = new GPPass_DepthLayer(this, DL::DepthLayerCanvas,
    {
      new GPPass_Selection(this)
    });
    m_gizmosSelectionPass  = new GPPass_DepthLayer(this, DL::DepthLayerGizmos,
    {
      new GPPass_Selection(this)
    });
    #endif
}

GraphicPipeline::~GraphicPipeline()
{
    delete m_gbuffer;
    delete m_scenePass;
    delete m_canvasPass;
    delete m_gizmosPass;
    delete m_fxaaPass;

    #ifdef BANG_EDITOR
    delete m_selectionFB;
    delete m_sceneSelectionPass;
    delete m_canvasSelectionPass;
    delete m_gizmosSelectionPass;
    #endif

    delete m_matSelectionEffectScreen;
    delete m_texUnitManager;
    delete m_glContext;
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

    #ifdef BANG_EDITOR
    if (!m_renderingInGame)
    {
        //c.MarkEvent("RenderSelectionBuffer");
        RenderSelectionBuffer(renderers, sceneChildren, p_scene);
        if (Input::GetKey(Input::Key::S))
        {
            RenderToScreen(m_selectionFB->GetColorTexture()); // To see selFB
        }
    }
    #endif
}

void GraphicPipeline::ApplySelectionOutline()
{
    #ifdef BANG_EDITOR
    if (!Hierarchy::GetInstance()->GetFirstSelectedGameObject()) { return; }

    List<GameObject*> sceneGameObjects = p_scene->GetChildrenRecursively();

    // Create stencil mask that the selection pass will use
    GL::SetTestDepth(false);
    m_gbuffer->Bind();
    m_gbuffer->ClearStencil();
    m_gbuffer->SetStencilTest(false);
    m_gbuffer->SetStencilWrite(true);
    m_gbuffer->SetAllDrawBuffersExceptColor();
    for (GameObject *go : sceneGameObjects)
    {
        if (go->IsSelected() && !go->transform->IsOfType<RectTransform>())
        {
            List<Renderer*> rends = go->GetComponents<Renderer>();
            for (Renderer *rend : rends)
            {
                if (rend && rend->IsEnabled()) { rend->Render(); }
            }
        }
    }

    // Apply selection outline
    m_gbuffer->ApplyPass(m_matSelectionEffectScreen->GetShaderProgram());
    m_gbuffer->UnBind();
    GL::SetTestDepth(true);
    #endif
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
    if (!m_renderingInGame) { ApplySelectionOutline(); }
    m_gbuffer->ClearStencil();
    if (GetFXAA()) { m_fxaaPass->Pass(renderers, sceneChildren); }

    m_canvasPass->Pass(renderers, sceneChildren);
    m_gizmosPass->Pass(renderers, sceneChildren);

    m_gbuffer->UnBind();
}

#ifdef BANG_EDITOR
void GraphicPipeline::RenderSelectionBuffer(
                        const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren,
                        Scene *scene)
{
    Camera *cam = p_scene->GetCamera();
    m_selectionFB->m_isPassing = true;
    m_selectionFB->PrepareForRender(scene);
    cam->SetReplacementShaderProgram(m_selectionFB->GetSelectionShaderProgram());

    m_selectionFB->Bind();

    m_selectionFB->ClearColor(Color::One);
    m_sceneSelectionPass->Pass (renderers, sceneChildren);
    m_canvasSelectionPass->Pass(renderers, sceneChildren);
    m_gizmosSelectionPass->Pass(renderers, sceneChildren);

    m_selectionFB->UnBind();

    cam->SetReplacementShaderProgram(nullptr);
    m_selectionFB->ProcessSelection();
    m_selectionFB->m_isPassing = false;
}
#endif

void GraphicPipeline::ApplyScreenPass(ShaderProgram *sp, const Rect &mask)
{
    sp->SetVec2("B_rectMinCoord", mask.GetMin());
    sp->SetVec2("B_rectMaxCoord", mask.GetMax());

    sp->Bind();
    RenderScreenPlane();
    sp->UnBind();
}

void GraphicPipeline::RenderToScreen(Texture *fullScreenTexture)
{
    ShaderProgram *sp = m_renderGBufferToScreenMaterial->GetShaderProgram();
    ENSURE(sp);

    m_renderGBufferToScreenMaterial->Bind();
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

#ifdef BANG_EDITOR
SelectionFramebuffer *GraphicPipeline::GetSelectionFramebuffer()
{
    return m_selectionFB;
}
#endif

GraphicPipeline* GraphicPipeline::GetActive()
{
    Screen *screen = Screen::GetInstance();
    return screen ? screen->GetGraphicPipeline() : nullptr;
}

void GraphicPipeline::OnResize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
    #ifdef BANG_EDITOR
    m_selectionFB->Resize(newWidth, newHeight);
    #endif
}

void GraphicPipeline::SetGBufferAttachmentToBeRendered(
        GBuffer::AttachmentId attachment)
{
    m_gbufferAttachToBeShown = attachment;
}

void GraphicPipeline::SetFXAA(bool enabled)
{
    m_FXAA = enabled;
}

bool GraphicPipeline::GetFXAA() const
{
    return m_FXAA;
}

GLContext *GraphicPipeline::GetGLContext() const
{
    return m_glContext;
}

GBuffer *GraphicPipeline::GetGBuffer()
{
    return m_gbuffer;
}

TextureUnitManager *GraphicPipeline::GetTextureUnitManager() const
{
    return m_texUnitManager;
}
