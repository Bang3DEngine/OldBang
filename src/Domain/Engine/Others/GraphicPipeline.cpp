#include "GraphicPipeline.h"

#include "Light.h"
#include "Screen.h"
#include "GBuffer.h"
#include "Material.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "Hierarchy.h"
#include "SelectionFramebuffer.h"
#endif

GraphicPipeline::GraphicPipeline(Screen *screen)
{
    m_matAmbientLightScreen    = AssetsManager::LoadAsset<Material>(
                "Assets/Engine/Materials/PR_AmbientLight_Screen.bmat");
    m_matSelectionEffectScreen = AssetsManager::LoadAsset<Material>(
                "Assets/Engine/Materials/PR_SelectionEffect.bmat");

    m_gbuffer = new GBuffer(screen->m_width, screen->m_height);
    #ifdef BANG_EDITOR
    m_selectionFB = new SelectionFramebuffer(screen->m_width, screen->m_height);
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
        bool immediatePostRender = (rend->IsTransparent()   ||
                                    rend->HasCustomPRPass() ||
                                    rend->IsGizmo());
        if (immediatePostRender)
        {
            m_gbuffer->ClearStencil();
        }

        m_gbuffer->SetStencilTest(false);
        m_gbuffer->SetStencilWrite(true);
        m_gbuffer->SetAllDrawBuffersExceptColor();

        rend->Render();

        if (immediatePostRender)
        {
            // These PR's are stenciled from the Render before
            ApplyDeferredLights(rend);
            if (rend->HasCustomPRPass())
            {
                RenderCustomPR(rend);
            }
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
    m_gbuffer->SetAllDrawBuffersExceptColor();

    m_gbuffer->ClearStencil();
    m_gbuffer->SetStencilTest(false);
    m_gbuffer->SetStencilWrite(true);
    for (GameObject *go : sceneGameObjects)
    {
        if (go->IsSelected())
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

    // Apply selection outline
    m_gbuffer->RenderPassWithMaterial(m_matSelectionEffectScreen);
    #endif
}

void GraphicPipeline::ApplyDeferredLights(Renderer *rend)
{
    // Limit to the rend visible rect, to save bandwidth
    Rect renderRect = Rect::ScreenRect;
    if (rend)
    {
        renderRect = rend->gameObject->GetBoundingScreenRect(
                            m_currentScene->GetCamera(), false);
    }

    renderRect = Rect::ScreenRect; // TODO: Erase this and correct rect creation, it doesnt work sometimes
    // If the rect is empty, dont waste time rendering nothing (which wastes time)
    if (renderRect != Rect::Empty)
    {
        m_gbuffer->SetStencilTest(true);
        m_gbuffer->RenderPassWithMaterial(m_matAmbientLightScreen, renderRect);
        if (!rend || rend->GetReceivesLighting())
        {
            List<Light*> lights = m_currentScene->GetComponentsInChildren<Light>();
            for (Light *light : lights)
            {
                if (CAN_USE_COMPONENT(light))
                {
                    light->ApplyLight(m_gbuffer, renderRect);
                }
            }
        }
    }
}

void GraphicPipeline::RenderPassWithDepthLayer(Renderer::DepthLayer depthLayer,
                                               Framebuffer *fb)
{
    m_currentDepthLayer = depthLayer;
    fb->ClearDepth(); // After each pass, clear the depth

    List<Renderer*> renderers = m_currentScene->GetComponentsInChildren<Renderer>();
    for (Renderer *rend : renderers) // Opaque
    {
        if (!rend->IsTransparent() && !rend->HasCustomPRPass() &&
            !rend->IsGizmo())
        {
            RenderRenderer(rend);
        }
    }

    if (fb == m_gbuffer)
    {
        ApplyDeferredLights();
        m_gbuffer->ClearStencil();
    }

    for (Renderer *rend : renderers) // Immediate PR
    {
        if ((rend->IsTransparent() || rend->HasCustomPRPass()) &&
            !rend->IsGizmo())
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

void GraphicPipeline::RenderCustomPR(Renderer *rend)
{
    if (!CAN_USE_COMPONENT(rend)) { return; }
    if (rend->GetDepthLayer() != m_currentDepthLayer) { return; }

    if (!m_selectionFB->IsPassing())
    {
        m_gbuffer->SetStencilWrite(false);
        m_gbuffer->SetStencilTest(true);
    }
    rend->RenderCustomPR();
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

    if (fb == m_gbuffer)
    {
        ApplySelectionEffect();
    }

    RenderGizmosOverlayPass(fb);
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

GraphicPipeline* GraphicPipeline::GetActive()
{
    Screen *screen = Screen::GetInstance();;
    return screen ? screen->GetGraphicPipeline() : nullptr;
}

SelectionFramebuffer *GraphicPipeline::GetSelectionFramebuffer() const
{
    return m_selectionFB;
}

void GraphicPipeline::OnResize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
    m_selectionFB->Resize(newWidth, newHeight);
}

GBuffer *GraphicPipeline::GetGBuffer() const
{
    return m_gbuffer;
}

Renderer::DepthLayer GraphicPipeline::GetCurrentDepthLayer() const
{
    return m_currentDepthLayer;
}

#endif
