#include "GraphicPipeline.h"

#include "Light.h"
#include "Screen.h"
#include "GBuffer.h"
#include "Material.h"
#include "GameObject.h"
#include "AssetsManager.h"

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

}

GraphicPipeline::~GraphicPipeline()
{
    delete m_gbuffer;
    delete m_matAfterLightingScreen;
    delete m_matBeforeLightingScreen;
}

GraphicPipeline* GraphicPipeline::GetActive()
{
    Scene *scene = SceneManager::GetActiveScene();
    return scene ? scene->GetGraphicPipeline() : nullptr;
}

void GraphicPipeline::RenderScene(Scene *scene)
{
    m_gbuffer->Bind();
    m_gbuffer->SetAllDrawBuffers(); // Prepare the buffers to be written

    // CLEAR. First, clear everything in gbuffer (depth and all its buffers)
    Color bgColor = scene->GetCamera()->GetClearColor();
    m_gbuffer->ClearBuffersAndBackground(bgColor);

    List<Renderer*> renderers = scene->GetComponentsInChildren<Renderer>();
    Gizmos::Reset(); // Disable Gizmos renderers

    // PART 1. Opaque objects
    for (Renderer *rend : renderers)
    {
        if (CAN_USE_COMPONENT(rend) && !rend->IsTransparent())
        {
            rend->Render();
        }
    }

    #ifdef BANG_EDITOR // Draw Gizmos
    PROPAGATE_EVENT(_OnDrawGizmos, scene->m_children);
    #endif
    m_gbuffer->RenderPassWithMaterial(m_matBeforeLightingScreen); // Put ambient light
    // ApplyDeferredLightsToAllGBuffer(scene);


    //PART 2. Transparent objects
    for (Renderer *rend : renderers)
    {
        if (CAN_USE_COMPONENT(rend) && rend->IsTransparent())
        {
            m_gbuffer->SetAllDrawBuffers();
            rend->Render();

            // glEnable(GL_BLEND);
            // glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // Only apply PR effects to the rendered zone of rend, not to all the screen
            m_gbuffer->SetDrawBuffers({GBuffer::Attachment::Color});
            rend->RenderWithMaterial(m_matBeforeLightingMesh);
            //ApplyDeferredLightsToRenderer(scene, rend);
        }
    }
    m_gbuffer->SetAllDrawBuffers();

    // BREAK. Clear the depth
    m_gbuffer->ClearDepth();
/*
    // PART 4. Opaque Gizmos with Deferred
    RenderOpaqueNoDepthGizmos(scene);

    // PART 5. Opaque Gizmos with Deferred
    // RenderTransparentNoDepthGizmos(scene);
*/

    // PART 3. PostRenderEffects
    // RenderPostRenderEffects(scene);

    m_gbuffer->UnBind();

    // FINAL. Render the gbuffer to the screen
    m_gbuffer->RenderToScreen(); // WORKING GOOD
    //Debug_Log("Ends render");
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
            light->ApplyLight(rend);
        }
    }
}

void GraphicPipeline::OnResize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
}
