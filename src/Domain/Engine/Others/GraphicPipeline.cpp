#include "GraphicPipeline.h"

#include "VAO.h"
#include "VBO.h"
#include "Mesh.h"
#include "Debug.h"
#include "Debug.h"
#include "Scene.h"
#include "Light.h"
#include "Chrono.h"
#include "Screen.h"
#include "Camera.h"
#include "GBuffer.h"
#include "Texture.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshFactory.h"
#include "SceneManager.h"
#include "TextureRender.h"
#include "ShaderProgram.h"
#include "AssetsManager.h"
#include "ShaderContract.h"

#ifdef BANG_EDITOR
#include "Hierarchy.h"
#include "SelectionFramebuffer.h"
#endif

GraphicPipeline::GraphicPipeline(Screen *screen)
{
    m_matSelectionEffectScreen = AssetsManager::Load<Material>(
                "Materials/PR_SelectionEffect.bmat", true);

    m_gbuffer = new GBuffer(screen->m_width, screen->m_height);
    #ifdef BANG_EDITOR
    m_selectionFB = new SelectionFramebuffer(screen->m_width, screen->m_height);
    #endif

    m_renderGBufferToScreenMaterial =
            AssetsManager::Load<Material>("Materials/RenderGBufferToScreen.bmat", true);
    m_planeMeshToRenderEntireScreen = MeshFactory::GetPlane();
}

GraphicPipeline::~GraphicPipeline()
{
    delete m_gbuffer;
    delete m_matSelectionEffectScreen;
    #ifdef BANG_EDITOR
    delete m_selectionFB;
    #endif
}

void GraphicPipeline::RenderScene(Scene *scene)
{
    m_currentScene = scene;

    RenderGBuffer();
    #ifdef BANG_EDITOR
    RenderSelectionFramebuffer();
    #endif

    m_gbuffer->RenderToScreen(m_gbufferAttachmentToBeShown);
    // RenderToScreen(m_selectionFB->GetColorTexture()); // Uncomment to see the framebuffer
}

void GraphicPipeline::RenderRenderer(Renderer *rend)
{
    if (!CAN_USE_COMPONENT(rend)) { return; }
    if (rend->GetDepthLayer() != m_currentDepthLayer) { return; }

    #ifdef BANG_EDITOR
    if (!m_selectionFB->IsPassing())
    #endif
    {
        // Rendering to GBuffer

        // immediatePostRender means we have to apply the light and possibly
        // other postRender effects immediately. For example, in transparent
        // objects we need to apply deferred light locally before we render another
        // stuff, because if we render another transparent object above it, we would
        // be blending wrong colors
        bool immediatePostRender = (rend->IsTransparent()   ||
                                    rend->HasCustomPRPass() ||
                                    rend->IsGizmo());
        if (immediatePostRender)
        {
            // If there's a renderer with a postRender effect,
            // then do not accumulate the stenciling. The stencil
            // accumulation is basically used to apply the deferred lighting
            glDepthMask(GL_FALSE); // Do not write to depth

            // We need a brand new stencil to later apply the custom effects
            m_gbuffer->ClearStencil();
        }

        m_gbuffer->SetStencilTest(false); // Don't want to be filtered by the stencil
        m_gbuffer->SetStencilWrite(true); // We are going to mark into the stencil (to later let the deferred lighting be applied here)
        m_gbuffer->SetAllDrawBuffers(); // But we DO write to diffuseColor (color != diffuseColor !!!)

        rend->Render(); // Render without writing to the final color buffer

        if (immediatePostRender) // In case we need to apply immediately some PR
        {
            // These PR's are stenciled from the Render before
            ApplyDeferredLights(rend); // Only apply lights to the needed zone
            if (rend->HasCustomPRPass())
            {
                RenderCustomPR(rend);
            }
            glDepthMask(GL_TRUE);
        }
    }
    #ifdef BANG_EDITOR
    else
    {
        m_selectionFB->RenderForSelectionBuffer(rend);
    }
    #endif
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
        if (!go->IsSelected()) { continue; }

        List<Renderer*> rends = go->GetComponents<Renderer>();
        for (Renderer *rend : rends)
        {
            if (!CAN_USE_COMPONENT(rend)) { continue; }
            rend->Render();
        }
    }

    // Apply selection outline
    m_gbuffer->RenderPassWithMaterial(m_matSelectionEffectScreen);
    #endif
}

void GraphicPipeline::ApplyDeferredLights(Renderer *rend)
{
    // Limit rendering to the renderer visible rect
    Rect renderRect = Rect::ScreenRect;
    Camera *sceneCam = m_currentScene->GetCamera();
    if (rend)
    {
        renderRect = rend->gameObject->GetBoundingScreenRect(sceneCam, false);
    }
    else
    {
        renderRect = m_currentScene->GetBoundingScreenRect(sceneCam, true);
    }


    ASSERT(renderRect != Rect::Empty); // If the rect is empty, dont waste time rendering nothing

    m_gbuffer->SetStencilTest(true); // We have marked from before the zone where we want to apply the effect

    if (!rend || rend->ReceivesLighting())
    {
        List<Light*> lights = m_currentScene->GetComponentsInChildren<Light>();
        for (Light *light : lights)
        {
            if (!CAN_USE_COMPONENT(light)) { continue; }
            light->ApplyLight(m_gbuffer, renderRect);
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
        if (!rend->IsTransparent() && !rend->HasCustomPRPass() && !rend->IsGizmo())
        {
            RenderRenderer(rend);
        }
    }

    // Apply deferred lights to opaque objects
    if (fb == m_gbuffer)
    {
        ApplyDeferredLights();
        m_gbuffer->ClearStencil();
    }

    // Post-render passes (screen passes).
    // Either is transparent or has a custom PR Pass (gizmos are left for the end...)
    for (Renderer *rend : renderers)
    {
        if ((rend->IsTransparent() || rend->HasCustomPRPass()) &&
             !rend->IsGizmo())
        {
            RenderRenderer(rend);
        }
    }
}

void GraphicPipeline::RenderGizmosPass(Framebuffer *fb)
{
    m_currentDepthLayer = Renderer::DepthLayer::DepthLayerGizmos;
    fb->ClearDepth(); // After each pass, clear the depth

    List<GameObject*> sceneGameObjects = m_currentScene->GetChildrenEditor();
    for (GameObject *go : sceneGameObjects)
    {
        go->_OnDrawGizmos();
    }

    fb->ClearDepth();

    for (GameObject *go : sceneGameObjects)
    {
        go->_OnDrawGizmosOverlay();
    }
}

void GraphicPipeline::RenderCustomPR(Renderer *rend)
{
    if (!CAN_USE_COMPONENT(rend)) { return; }
    if (rend->GetDepthLayer() != m_currentDepthLayer) { return; }

    #ifdef BANG_EDITOR
    if (!m_selectionFB->IsPassing())
    #endif
    {
        m_gbuffer->SetStencilWrite(false);
        m_gbuffer->SetStencilTest(true);
    }
    rend->RenderCustomPR();
}

void GraphicPipeline::RenderPassWithMaterial(Material *mat, const Rect &renderRect)
{
    ASSERT(mat);

    m_planeMeshToRenderEntireScreen->
            BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                         *(mat->GetShaderProgram()));

    mat->GetShaderProgram()->SetUniformVec2("B_rectMinCoord", renderRect.GetMin());
    mat->GetShaderProgram()->SetUniformVec2("B_rectMaxCoord", renderRect.GetMax());

    mat->Bind();
    RenderScreenPlane();
    mat->UnBind();
}

void GraphicPipeline::RenderToScreen(Texture *fullScreenTexture)
{
    m_planeMeshToRenderEntireScreen->
                BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                             *(m_renderGBufferToScreenMaterial->GetShaderProgram()));

    ShaderProgram *sp = m_renderGBufferToScreenMaterial->GetShaderProgram(); ASSERT(sp);
    sp->SetUniformTexture("B_color_gout_fin", fullScreenTexture, false);

    m_renderGBufferToScreenMaterial->Bind();
    GraphicPipeline::RenderScreenPlane();
    m_renderGBufferToScreenMaterial->UnBind();
}

void GraphicPipeline::RenderScreenPlane()
{
    m_planeMeshToRenderEntireScreen->GetVAO()->Bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthFunc(GL_ALWAYS);
    glDepthMask(GL_FALSE);
    glDrawArrays(GL_TRIANGLES, 0, m_planeMeshToRenderEntireScreen->GetVertexCount());
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    m_planeMeshToRenderEntireScreen->GetVAO()->UnBind();
}

void GraphicPipeline::RenderGBuffer()
{
    Color bgColor = m_currentScene->GetCamera()->GetClearColor();
    m_gbuffer->ClearBuffersAndBackground(bgColor);

    m_gbuffer->Bind();
    m_gbuffer->SetAllDrawBuffers();

    RenderPassWithDepthLayer(Renderer::DepthLayer::DepthLayerScene, m_gbuffer);
    RenderPassWithDepthLayer(Renderer::DepthLayer::DepthLayerCanvas, m_gbuffer);
    ApplySelectionEffect();
    RenderGizmosPass(m_gbuffer);

    m_gbuffer->UnBind();

}

#ifdef BANG_EDITOR
void GraphicPipeline::RenderSelectionFramebuffer()
{
    m_selectionFB->m_isPassing = true;
    m_selectionFB->PrepareForRender(m_currentScene);

    m_selectionFB->Bind();
    m_selectionFB->Clear();

    RenderPassWithDepthLayer(Renderer::DepthLayer::DepthLayerScene, m_selectionFB);
    RenderPassWithDepthLayer(Renderer::DepthLayer::DepthLayerCanvas, m_selectionFB);
    RenderGizmosPass(m_selectionFB);

    m_selectionFB->UnBind();

    m_selectionFB->ProcessSelection();
    m_selectionFB->m_isPassing = false;
}

SelectionFramebuffer *GraphicPipeline::GetSelectionFramebuffer() const
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

void GraphicPipeline::SetGBufferAttachmentToBeRendered(GBuffer::Attachment attachment)
{
    m_gbufferAttachmentToBeShown = attachment;
}

GBuffer *GraphicPipeline::GetGBuffer() const
{
    return m_gbuffer;
}

Renderer::DepthLayer GraphicPipeline::GetCurrentDepthLayer() const
{
    return m_currentDepthLayer;
}
