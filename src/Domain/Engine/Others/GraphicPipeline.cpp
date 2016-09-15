#include "GraphicPipeline.h"

#include "Light.h"
#include "Screen.h"
#include "GBuffer.h"
#include "Material.h"
#include "GameObject.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "SelectionFramebuffer.h"
#endif

GraphicPipeline::GraphicPipeline()
{
    m_gbuffer = new GBuffer(Screen::GetWidth(), Screen::GetHeight());

    String drawScreenPlaneVert = "Assets/Engine/Shaders/PR_DrawScreenPlane.vert";
    String beforeLightingFrag  = "Assets/Engine/Shaders/PR_BeforeLighting.frag";
    String afterLightingFrag  = "Assets/Engine/Shaders/PR_AfterLighting.frag";
    String meshVert = "Assets/Engine/Shaders/PR_Mesh.vert";

    m_matBeforeLightingScreen = new Material();
    m_matBeforeLightingScreen->SetShaderProgram(new ShaderProgram(drawScreenPlaneVert, beforeLightingFrag));

    m_matAfterLightingScreen  = new Material();
    m_matAfterLightingScreen->SetShaderProgram(new ShaderProgram(drawScreenPlaneVert, afterLightingFrag));

    m_matBeforeLightingMesh   = new Material();
    m_matBeforeLightingMesh->SetShaderProgram(new ShaderProgram(meshVert, beforeLightingFrag));

    m_matAfterLightingMesh    = new Material();
    m_matAfterLightingMesh->SetShaderProgram(new ShaderProgram(meshVert, afterLightingFrag));

    #ifdef BANG_EDITOR
    m_selectionFB = new SelectionFramebuffer(Screen::GetWidth(), Screen::GetHeight());
    #endif
}

GraphicPipeline::~GraphicPipeline()
{
    delete m_gbuffer;
    delete m_matAfterLightingScreen;
    delete m_matBeforeLightingScreen;
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
    m_gbuffer->Bind();

    #ifdef BANG_EDITOR
    Gizmos::Reset(); // Disable Gizmos renderers
    #endif

    // CLEAR. First, clear everything in gbuffer (depth and all its buffers)
    Color bgColor = scene->GetCamera()->GetClearColor();
    m_gbuffer->ClearBuffersAndBackground(bgColor);

    RenderOpaque(scene);
    RenderTransparent(scene);

    #ifdef BANG_EDITOR
    m_gbuffer->ClearDepth();
    RenderNoDepth(scene);
    #endif

    RenderPostRenderEffects(scene); // PostRenderEffects (selection outline for example)

    m_gbuffer->UnBind();

    m_gbuffer->RenderToScreen();

    /*
    static int x = 0, N = 40; ++x;
    if      (x < N * 1) m_gbuffer->RenderToScreen(GBuffer::Attachment::Position);
    else if (x < N * 2) m_gbuffer->RenderToScreen(GBuffer::Attachment::Normal);
    else if (x < N * 3) m_gbuffer->RenderToScreen(GBuffer::Attachment::Uv);
    else if (x < N * 4) m_gbuffer->RenderToScreen(GBuffer::Attachment::Diffuse);
    else if (x < N * 5) m_gbuffer->RenderToScreen(GBuffer::Attachment::MaterialProperties);
    else if (x < N * 6) m_gbuffer->RenderToScreen(GBuffer::Attachment::Depth);
    else if (x < N * 7) m_gbuffer->RenderToScreen(GBuffer::Attachment::Color);
    else x = 0;
    */

    // RENDER SELECTION FRAMEBUFFER
    #ifdef BANG_EDITOR
    RenderSelectionFramebuffer(scene);
    // Uncomment if u want to see selectionFramebuffer
    // glClear(GL_DEPTH_BUFFER_BIT);
    // m_selectionFB->RenderSelectionBuffer(scene);
    #endif
}

void GraphicPipeline::RenderOpaque(Scene *scene)
{
    m_transparentPass = false;
    m_gbuffer->SetAllDrawBuffersExceptColor();
    List<Renderer*> renderers = scene->GetComponentsInChildren<Renderer>();
    for (Renderer *rend : renderers)
    {
        if (CAN_USE_COMPONENT(rend) && !rend->IsTransparent())
        {
            rend->Render();
        }
    }

    #ifdef BANG_EDITOR
    for (GameObject *go : scene->m_children)
    {
        go->_OnDrawGizmos();
    }
    #endif

    ApplyPREffectsToScreen(scene);
}

void GraphicPipeline::RenderTransparent(Scene *scene)
{
    m_transparentPass = true;
    List<Renderer*> renderers = scene->GetComponentsInChildren<Renderer>();
    for (Renderer *rend : renderers)
    {
        if (CAN_USE_COMPONENT(rend) && rend->IsTransparent())
        {
            m_gbuffer->SetAllDrawBuffersExceptColor();
            rend->Render();
            ApplyPREffectsToRenderer(rend);
        }
    }

    #ifdef BANG_EDITOR
    m_gbuffer->SetAllDrawBuffersExceptColor();
    for (GameObject *go : scene->m_children)
    {
        go->_OnDrawGizmos();
        // Themselves apply the PR (from Gizmos::Render func)
    }
    #endif
}

void GraphicPipeline::RenderNoDepth(Scene *scene)
{
    m_transparentPass = false;
    m_gbuffer->SetAllDrawBuffersExceptColor();
    for (GameObject *go : scene->m_children)
    {
        go->_OnDrawGizmosNoDepth();
    }
    ApplyPREffectsToScreen(scene);
}

void GraphicPipeline::RenderPostRenderEffects(Scene *scene)
{
    #ifdef BANG_EDITOR
    m_gbuffer->RenderPassWithMaterial(m_matAfterLightingScreen);
    #endif
}

void GraphicPipeline::ApplyDeferredLightsToAllGBuffer(Scene *scene)
{
    List<Light*> childrenLights = scene->GetComponentsInChildren<Light>();
    for (Light *light : childrenLights)
    {
        if (CAN_USE_COMPONENT(light))
        {
            light->ApplyLight(m_gbuffer);
        }
    }
}

void GraphicPipeline::ApplyDeferredLightsToRenderer(Scene *scene, Renderer *rend)
{
    List<Light*> childrenLights = scene->GetComponentsInChildren<Light>();
    for (Light *light : childrenLights)
    {
        if (CAN_USE_COMPONENT(light))
        {
            light->ApplyLight(m_gbuffer, rend);
        }
    }
}

void GraphicPipeline::ApplyPREffectsToScreen(Scene *scene)
{
    m_gbuffer->RenderPassWithMaterial(m_matBeforeLightingScreen);
    ApplyDeferredLightsToAllGBuffer(scene);
}

void GraphicPipeline::OnResize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
    #ifdef BANG_EDITOR
    m_selectionFB->Resize(newWidth, newHeight);
    #endif
}

void GraphicPipeline::ApplyPREffectsToRenderer(Renderer *renderer)
{
    #ifdef BANG_EDITOR
    if (m_selectionFB->IsPassing()) { return; } // If SFB passing, dont apply PR
    #endif

    Scene *scene = SceneManager::GetActiveScene();

    // Only apply PR effects to the rendered zone of rend, not to all the screen
    m_gbuffer->SetColorDrawBuffer();
    m_gbuffer->BindInputTexturesTo(m_matBeforeLightingMesh);
    renderer->RenderWithMaterial(m_matBeforeLightingMesh);
    ApplyDeferredLightsToRenderer(scene, renderer);
}

#ifdef BANG_EDITOR
void GraphicPipeline::RenderSelectionFramebuffer(Scene *scene)
{
    m_selectionFB->Bind();
    m_selectionFB->Clear();
    m_selectionFB->RenderSelectionBuffer(scene);
    m_selectionFB->UnBind();

    m_selectionFB->ProcessSelection();
}

SelectionFramebuffer *GraphicPipeline::GetSelectionFramebuffer() const
{
    return m_selectionFB;
}
#endif

bool GraphicPipeline::IsTransparentPass() const
{
    return m_transparentPass;
}
