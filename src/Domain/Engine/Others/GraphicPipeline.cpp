#include "GraphicPipeline.h"

#include "Light.h"
#include "Screen.h"
#include "GBuffer.h"
#include "Material.h"
#include "AssetsManager.h"

GraphicPipeline::GraphicPipeline()
{
    m_gbuffer = new GBuffer(Screen::GetWidth(), Screen::GetHeight());
    m_materialBeforeLighting = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/PR_BeforeLighting.bmat");
    m_materialAfterLighting  = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/PR_AfterLighting.bmat");
}

GraphicPipeline::~GraphicPipeline()
{
    delete m_gbuffer;
    delete m_materialAfterLighting;
    delete m_materialBeforeLighting;
}

GraphicPipeline* GraphicPipeline::GetActive()
{
    Scene *scene = SceneManager::GetActiveScene();
    return scene ? scene->GetGraphicPipeline() : nullptr;
}

void GraphicPipeline::RenderScene(Scene *scene)
{
    m_gbuffer->Bind();

    // CLEAR. First, clear everything in gbuffer (depth and all its buffers)
    Color bgColor = scene->GetCamera()->GetClearColor();
    m_gbuffer->ClearBuffersAndBackground(bgColor);

    m_gbuffer->SetAllDrawBuffers(); // Prepare the buffers to be written

    // PART 1. Opaque with Deferred
    RenderOpaque(scene);

    // PART 2. Transparent with Forward
    // RenderTransparent(scene);

    // BREAK. Clear the depth
    m_gbuffer->ClearDepth();

    // PART 4. Opaque Gizmos with Deferred
    RenderOpaqueNoDepthGizmos(scene);

    // PART 5. Opaque Gizmos with Deferred
    // RenderTransparentNoDepthGizmos(scene);

    // PART 3. PostRenderEffects
    RenderPostRenderEffects(scene);

    m_gbuffer->UnBind();

    // FINAL. Render the gbuffer to the screen
    m_gbuffer->RenderToScreen();
}

void GraphicPipeline::RenderOpaque(Scene *scene)
{
    m_opaquePass = true;

    // First, we fill in the GBuffer with the positions, normals, etc.
    // D2G (DrawToGBuffer)
    PROPAGATE_EVENT(_OnPreRender, scene->m_children);
    PROPAGATE_EVENT(_OnRender, scene->m_children);

    // Draw Gizmos!
    #ifdef BANG_EDITOR
    PROPAGATE_EVENT(_OnDrawGizmos, scene->m_children);
    #endif
    //

    // Add ambient light
    m_gbuffer->RenderPassWithMaterial(m_materialBeforeLighting);
    ApplyDeferredLights(scene);
}

void GraphicPipeline::RenderTransparent(Scene *scene)
{
    // Here we use Forward rendering.
    // So lights are applied in the same shader
    m_opaquePass = false;

    // Enable Blend to allow transparency
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (GameObject *child : scene->m_children)
    {
        child->_OnPreRender();
        child->_OnRender();
        child->_OnDrawGizmos();
    }

    glDisable(GL_BLEND);
}

void GraphicPipeline::RenderPostRenderEffects(Scene *scene)
{
    #ifdef BANG_EDITOR
    m_gbuffer->RenderPassWithMaterial(m_materialAfterLighting);
    #endif
}

void GraphicPipeline::RenderOpaqueNoDepthGizmos(Scene *scene)
{
    m_opaquePass = true;
    PROPAGATE_EVENT(_OnDrawGizmosNoDepth, scene->m_children);

    // Add ambient light
    m_gbuffer->RenderPassWithMaterial(m_materialBeforeLighting);
    ApplyDeferredLights(scene);
}

void GraphicPipeline::RenderTransparentNoDepthGizmos(Scene *scene)
{
    m_opaquePass = false;
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    PROPAGATE_EVENT(_OnDrawGizmosNoDepth, scene->m_children);
    glDisable(GL_BLEND);
}

void GraphicPipeline::ApplyDeferredLights(Scene *scene)
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

void GraphicPipeline::OnResize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
}

bool GraphicPipeline::IsInOpaquePass() const
{
    return m_opaquePass;
}
