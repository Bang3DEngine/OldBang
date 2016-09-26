#include "GraphicPipeline.h"

#include "Light.h"
#include "Screen.h"
#include "GBuffer.h"
#include "Material.h"
#include "GameObject.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "Hierarchy.h"
#include "SelectionFramebuffer.h"
#endif

GraphicPipeline::GraphicPipeline()
{
    m_gbuffer = new GBuffer(Screen::GetWidth(), Screen::GetHeight());

    String drawScreenPlaneVert = "Assets/Engine/Shaders/PR_DrawScreenPlane.vert";
    String ambientLightFrag    = "Assets/Engine/Shaders/PR_AmbientLight.frag";
    String afterLightingFrag   = "Assets/Engine/Shaders/PR_SelectionEffect.frag";

    m_matAmbientLightScreen = new Material();
    m_matAmbientLightScreen->SetShaderProgram(new ShaderProgram(drawScreenPlaneVert, ambientLightFrag));

    m_matSelectionEffectScreen  = new Material();
    m_matSelectionEffectScreen->SetShaderProgram(new ShaderProgram(drawScreenPlaneVert, afterLightingFrag));

    #ifdef BANG_EDITOR
    m_selectionFB = new SelectionFramebuffer(Screen::GetWidth(), Screen::GetHeight());
    #endif
}

GraphicPipeline::~GraphicPipeline()
{
    delete m_gbuffer;
    delete m_matSelectionEffectScreen;
    delete m_matAmbientLightScreen;
    #ifdef BANG_EDITOR
    delete m_selectionFB;
    #endif
}

GraphicPipeline* GraphicPipeline::GetActive()
{
    Scene *scene = SceneManager::GetActiveScene();
    return scene ? scene->GetGraphicPipeline() : nullptr;
}

void GraphicPipeline::RenderScene(Scene *scene)
{
    m_currentScene = scene;

    Color bgColor = m_currentScene->GetCamera()->GetClearColor();
    m_gbuffer->ClearBuffersAndBackground(bgColor);
    m_gbuffer->SetAllDrawBuffersExceptColor();
    m_gbuffer->Bind();
    RenderDepthLayers(m_gbuffer);
    m_gbuffer->UnBind();
    m_gbuffer->RenderToScreen();

    #ifdef BANG_EDITOR
    RenderSelectionFramebuffer();
    #endif
}

void GraphicPipeline::RenderRenderer(Renderer *rend)
{
    if (!CAN_USE_COMPONENT(rend)) { return; }
    if (rend->GetDepthLayer() != m_currentDepthLayer) { return; }

    if (!m_selectionFB->IsPassing())
    {
        bool immediatePostRender = (rend->IsTransparent() || rend->IsGizmo());
        if (immediatePostRender)
        {
            m_gbuffer->ClearStencil();
            m_gbuffer->SetStencilWrite(true);
            m_gbuffer->SetStencilTest(false);
        }

        rend->Render();

        if (immediatePostRender)
        {
           m_gbuffer->SetStencilWrite(false);
           m_gbuffer->SetStencilTest(true);
           ApplyDeferredLightsToScreen();
           m_gbuffer->SetStencilTest(false);
        }
    }
    else
    {
        m_selectionFB->RenderForSelectionBuffer(rend);
    }
}

void GraphicPipeline::ApplySelectionEffect()
{
    #ifdef BANG_EDITOR

    if (!Hierarchy::GetInstance()->GetFirstSelectedGameObject()) { return; }

    List<GameObject*> sceneGameObjects =
            m_currentScene->GetChildrenRecursivelyEditor();

    // Create stencil mask that the selection pass will use
    m_gbuffer->ClearDepth();
    m_gbuffer->ClearStencil();
    m_gbuffer->SetAllDrawBuffersExceptColor();
    m_gbuffer->SetStencilWrite(true);
    m_gbuffer->SetStencilTest(false);
    for (GameObject *go : sceneGameObjects)
    {
        if (go->IsSelectedInHierarchy())
        {
            List<Renderer*> rends = go->GetComponentsInThisAndChildren<Renderer>();
            for (Renderer *rend : rends)
            {
                if (CAN_USE_COMPONENT(rend))
                {
                    rend->Render();
                }
            }
        }
    }
    m_gbuffer->SetStencilWrite(false);

    // Apply selection outline
    m_gbuffer->RenderPassWithMaterial(m_matSelectionEffectScreen);

    m_gbuffer->ClearDepth();
    m_gbuffer->ClearStencil();

    #endif
}

void GraphicPipeline::ApplyDeferredLightsToScreen()
{
    m_gbuffer->SetStencilTest(true);
    m_gbuffer->SetStencilWrite(false);
    m_gbuffer->RenderPassWithMaterial(m_matAmbientLightScreen);
    List<Light*> lights = m_currentScene->GetComponentsInChildren<Light>();
    for (Light *light : lights)
    {
        if (CAN_USE_COMPONENT(light))
        {
            light->ApplyLight(m_gbuffer);
        }
    }
    m_gbuffer->SetStencilTest(false);
    m_gbuffer->SetStencilWrite(true);
}

#ifdef BANG_EDITOR
void GraphicPipeline::RenderSelectionFramebuffer()
{
    m_selectionFB->m_isPassing = true;
    m_selectionFB->PrepareForRender(m_currentScene);

    m_selectionFB->Bind();
    m_selectionFB->Clear();
    RenderDepthLayers(m_selectionFB);
    m_selectionFB->UnBind();

    m_selectionFB->ProcessSelection();
    m_selectionFB->m_isPassing = false;
}

void GraphicPipeline::RenderPassWithDepthLayer(Renderer::DepthLayer depthLayer,
                                               Framebuffer *fb)
{
    m_currentDepthLayer = depthLayer;
    fb->ClearDepth(); // After each pass, clear the depth

    List<Renderer*> renderers = m_currentScene->GetComponentsInChildren<Renderer>();
    for (Renderer *rend : renderers) // Opaque
    {
        if (!rend->IsTransparent() && !rend->IsGizmo())
        {
            RenderRenderer(rend);
        }
    }
    if (fb == m_gbuffer) ApplyDeferredLightsToScreen();

    for (Renderer *rend : renderers) // Transparent
    {
        if (rend->IsTransparent() && !rend->IsGizmo())
        {
            RenderRenderer(rend);
        }
    }

    List<GameObject*> sceneGameObjects = m_currentScene->GetChildrenEditor();
    for (GameObject *go : sceneGameObjects)
    {
        go->_OnDrawGizmos();
    }
}

void GraphicPipeline::RenderGizmosOverlayPass(Framebuffer *fb)
{
    m_currentDepthLayer = Renderer::DepthLayer::DepthLayerGizmosOverlay;
    fb->ClearDepth(); // After each pass, clear the depth

    List<GameObject*> sceneGameObjects = m_currentScene->GetChildrenEditor();
    for (GameObject *go : sceneGameObjects)
    {
        go->_OnDrawGizmosOverlay();
    }
}

void GraphicPipeline::RenderDepthLayers(Framebuffer *fb)
{
    for (Renderer::DepthLayer depthLayer : DepthLayerOrder)
    {
        if (depthLayer != Renderer::DepthLayer::DepthLayerGizmosOverlay)
        {
            RenderPassWithDepthLayer(depthLayer, fb);
        }
    }
    if (fb == m_gbuffer) ApplySelectionEffect();
    RenderGizmosOverlayPass(fb);
}

void GraphicPipeline::OnResize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
    #ifdef BANG_EDITOR
    m_selectionFB->Resize(newWidth, newHeight);
    #endif
}

GBuffer *GraphicPipeline::GetGBuffer() const
{
    return m_gbuffer;
}

SelectionFramebuffer *GraphicPipeline::GetSelectionFramebuffer() const
{
    return m_selectionFB;
}
#endif
