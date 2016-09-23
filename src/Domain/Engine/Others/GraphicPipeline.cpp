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

static int pass = 0;
static int passLimit = 1    ;

GraphicPipeline::GraphicPipeline()
{
    m_gbuffer = new GBuffer(Screen::GetWidth(), Screen::GetHeight());

    String drawScreenPlaneVert = "Assets/Engine/Shaders/PR_DrawScreenPlane.vert";
    String ambientLightFrag    = "Assets/Engine/Shaders/PR_AmbientLight.frag";
    String afterLightingFrag   = "Assets/Engine/Shaders/PR_EditorEffects.frag";

    m_matAmbientLightScreen = new Material();
    m_matAmbientLightScreen->SetShaderProgram(new ShaderProgram(drawScreenPlaneVert, ambientLightFrag));

    m_matEditorEffectsScreen  = new Material();
    m_matEditorEffectsScreen->SetShaderProgram(new ShaderProgram(drawScreenPlaneVert, afterLightingFrag));

    #ifdef BANG_EDITOR
    m_selectionFB = new SelectionFramebuffer(Screen::GetWidth(), Screen::GetHeight());
    #endif
}

GraphicPipeline::~GraphicPipeline()
{
    delete m_gbuffer;
    delete m_matEditorEffectsScreen;
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
    pass = 0;

    if (Input::GetKeyDown(Input::Key::Up))
    {
        ++passLimit;
    }
    else if (Input::GetKeyDown(Input::Key::Down))
    {
        --passLimit;
    }

    Gizmos::Reset();
    m_currentScene = scene;

    // CLEAR. First, clear everything in gbuffer (depth and all its buffers)
    Color bgColor = m_currentScene->GetCamera()->GetClearColor();
    m_gbuffer->ClearBuffersAndBackground(bgColor);

    m_gbuffer->Bind();
    List<GameObject*> sceneGameObjects = m_currentScene->GetChildrenEditor();
    List<Renderer*> renderers = m_currentScene->GetComponentsInChildren<Renderer>();
    for (Renderer::DepthLayer depthLayer : DepthLayerOrder)
    {
        m_currentDepthLayer = depthLayer;

        // After each pass, only the color remains
        m_gbuffer->ClearDepth();
        m_gbuffer->ClearAllBuffersExceptColor();

        if (m_currentDepthLayer != Renderer::DepthLayer::DepthLayerGizmosOverlay)
        {
            /*glEnable(GL_STENCIL_TEST);
            m_gbuffer->ClearStencil();
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            */
            // Opaque
            for (Renderer *rend : renderers)
            {
                if (!rend->IsTransparent() && !rend->IsGizmo())
                {
                    RenderRenderer(rend);
                }
            }
            ApplyDeferredLightsToScreen();
           /* glStencilFunc(GL_EQUAL, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glDisable(GL_STENCIL_TEST);
            */

            // Transparent
            for (Renderer *rend : renderers)
            {
                if (rend->IsTransparent() && !rend->IsGizmo())
                {
                    RenderRenderer(rend);
                }
            }

            /*
            m_gbuffer->UnBind();
            if (m_currentDepthLayer == Renderer::DepthLayer::DepthLayerScene)
            {
                static int N = 7, x = 0;
                if (Input::GetKeyDown(Input::Key::Right))     x = (x + 1 + N) % N;
                else if (Input::GetKeyDown(Input::Key::Left)) x = (x - 1 + N) % N;

                if (x == 0) { m_gbuffer->RenderToScreen(GBuffer::Attachment::Position); }
                else if (x == 1) { m_gbuffer->RenderToScreen(GBuffer::Attachment::Normal); }
                else if (x == 2) { m_gbuffer->RenderToScreen(GBuffer::Attachment::Uv); }
                else if (x == 3) { m_gbuffer->RenderToScreen(GBuffer::Attachment::Diffuse); }
                else if (x == 4) { m_gbuffer->RenderToScreen(GBuffer::Attachment::MaterialProperties); }
                else if (x == 5) { m_gbuffer->RenderToScreen(GBuffer::Attachment::Depth); }
                else if (x == 6) { m_gbuffer->RenderToScreen(GBuffer::Attachment::Color); }
            }
            m_gbuffer->Bind();
            */

            for (GameObject *go : sceneGameObjects)
            {
                go->_OnDrawGizmos();
            }
        }
        else
        {
            for (GameObject *go : sceneGameObjects)
            {
                go->_OnDrawGizmosNoDepth();
            }
        }

        ApplyEditorEffects();

        // uncomment to see all gbuffer attachments over time
        /*
        if (depthLayer == Renderer::DepthLayer::DepthLayerScene)
        {
            m_gbuffer->UnBind();
            static int x = 0, N = 40; ++x;
            if      (x < N * 1) m_gbuffer->RenderToScreen(GBuffer::Attachment::Position);
            else if (x < N * 2) m_gbuffer->RenderToScreen(GBuffer::Attachment::Normal);
            else if (x < N * 3) m_gbuffer->RenderToScreen(GBuffer::Attachment::Uv);
            else if (x < N * 4) m_gbuffer->RenderToScreen(GBuffer::Attachment::Diffuse);
            else if (x < N * 5) m_gbuffer->RenderToScreen(GBuffer::Attachment::MaterialProperties);
            else if (x < N * 6) m_gbuffer->RenderToScreen(GBuffer::Attachment::Depth);
            else if (x < N * 7) m_gbuffer->RenderToScreen(GBuffer::Attachment::Color);
            else x = 0;
            m_gbuffer->Bind();
        }
        */
    }

    m_gbuffer->UnBind();
    m_gbuffer->RenderToScreen();

    // Render SelectionFramebuffer
    #ifdef BANG_EDITOR
    RenderSelectionFramebuffer(renderers);
    #endif
}

void GraphicPipeline::RenderRenderer(Renderer *rend)
{
    if (!CAN_USE_COMPONENT(rend)) { return; }
    if (rend->GetDepthLayer() != m_currentDepthLayer) { return; }

    bool immediatePostRender = (rend->IsTransparent() || rend->IsGizmo());
    if (immediatePostRender)
    {
        /*m_gbuffer->ClearStencil();
        //glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        */
    }

    m_gbuffer->SetAllDrawBuffersExceptColor();
    rend->Render();

    if (immediatePostRender)
    {
       //glStencilFunc(GL_EQUAL, 1, 0xFF);
       // glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
       ApplyDeferredLightsToScreen();
       // glDisable(GL_STENCIL_TEST);
    }
}

void GraphicPipeline::ApplyEditorEffects()
{
    #ifdef BANG_EDITOR
    m_gbuffer->RenderPassWithMaterial(m_matEditorEffectsScreen);
    #endif
}

void GraphicPipeline::ApplyDeferredLightsToScreen()
{
    m_gbuffer->RenderPassWithMaterial(m_matAmbientLightScreen);
    List<Light*> lights = m_currentScene->GetComponentsInChildren<Light>();
    for (Light *light : lights)
    {
        if (CAN_USE_COMPONENT(light))
        {
            light->ApplyLight(m_gbuffer);
        }
    }
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

#ifdef BANG_EDITOR
void GraphicPipeline::RenderSelectionFramebuffer(const List<Renderer*> &renderers)
{
    m_selectionFB->PrepareForRender(m_currentScene);

    m_selectionFB->Bind();
    m_selectionFB->Clear();
    for (Renderer::DepthLayer depthLayer : DepthLayerOrder)
    {
        m_selectionFB->ClearDepth();
        for (Renderer *rend : renderers)
        {
            if (CAN_USE_COMPONENT(rend) &&
                rend->GetDepthLayer() == depthLayer)
            {
                m_selectionFB->RenderForSelectionBuffer(rend);
            }
        }
    }
    m_selectionFB->UnBind();

    m_selectionFB->ProcessSelection();
}

SelectionFramebuffer *GraphicPipeline::GetSelectionFramebuffer() const
{
    return m_selectionFB;
}
#endif
