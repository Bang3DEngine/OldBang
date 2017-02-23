
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
#include "GLContext.h"
#include "GameObject.h"
#include "MeshFactory.h"
#include "SceneManager.h"
#include "RenderTexture.h"
#include "ShaderProgram.h"
#include "AssetsManager.h"
#include "ShaderContract.h"
#include "GPPass_G_Gizmos.h"
#include "GPPass_G_OpaqueSP.h"
#include "GPPass_G_OpaqueNoSP.h"
#include "GraphicPipelineDebugger.h"
#include "GPPass_SP_DeferredLights.h"

#ifdef BANG_EDITOR
#include "Hierarchy.h"
#include "GPPass_Selection.h"
#include "SelectionFramebuffer.h"
#endif

GraphicPipeline::GraphicPipeline(Screen *screen)
  : m_gbuffer(new GBuffer(screen->m_width, screen->m_height)),
    #ifdef BANG_EDITOR
    m_selectionFB(new SelectionFramebuffer(screen->m_width, screen->m_height)),
    #endif
    m_scenePass (this, Renderer::DepthLayer::DepthLayerScene),
    m_canvasPass(this, Renderer::DepthLayer::DepthLayerCanvas),
    m_gizmosPass(this, Renderer::DepthLayer::DepthLayerGizmos)
    #ifdef BANG_EDITOR
   ,m_sceneSelectionPass (this, Renderer::DepthLayer::DepthLayerScene),
    m_canvasSelectionPass(this, Renderer::DepthLayer::DepthLayerCanvas),
    m_gizmosSelectionPass(this, Renderer::DepthLayer::DepthLayerGizmos)
    #endif
{
    m_glContext = new GLContext();

    m_matSelectionEffectScreen = AssetsManager::Load<Material>(
                "Materials/SP_SelectionEffect.bmat", true);

    m_renderGBufferToScreenMaterial =
         AssetsManager::Load<Material>("Materials/RenderGBufferToScreen.bmat",
                                       true);
    m_screenPlaneMesh = MeshFactory::GetUIPlane();

    // Set up graphic pipeline passes
    m_scenePass.AddSubPass (new GPPass_G_OpaqueSP(this));
    m_scenePass.AddSubPass (new GPPass_SP_DeferredLights(this));
    m_scenePass.AddSubPass (new GPPass_G_OpaqueNoSP(this));
    m_canvasPass.AddSubPass(new GPPass_G_OpaqueNoSP(this));
    m_gizmosPass.AddSubPass(new GPPass_G_Gizmos(this));

    #ifdef BANG_EDITOR
    m_sceneSelectionPass.AddSubPass(new GPPass_Selection(this));
    m_canvasSelectionPass.AddSubPass(new GPPass_Selection(this));
    m_gizmosSelectionPass.AddSubPass(new GPPass_Selection(this));
    #endif
}

GraphicPipeline::~GraphicPipeline()
{
    delete m_gbuffer;
    delete m_matSelectionEffectScreen;

    delete m_glContext;

    #ifdef BANG_EDITOR
    delete m_selectionFB;
    #endif
}

void GraphicPipeline::RenderScene(Scene *scene, bool inGame)
{
    p_scene = scene; ASSERT(p_scene);
    m_renderingInGame = inGame;

    List<Renderer*> renderers = scene->GetComponentsInChildren<Renderer>();
    List<GameObject*> sceneChildren = scene->GetChildren();

    // GBuffer
    Color bgColor = p_scene->GetCamera()->GetClearColor();
    m_gbuffer->ClearBuffersAndBackground(bgColor);
    m_gbuffer->Bind();

    m_scenePass.Pass(renderers, sceneChildren);
    m_canvasPass.Pass(renderers, sceneChildren);
    m_gizmosPass.Pass(renderers, sceneChildren);

    m_gbuffer->UnBind();
    m_gbuffer->RenderToScreen(m_gbufferAttachToBeShown);
    //

    #ifdef BANG_EDITOR
    // Selection buffer
    Camera *cam = p_scene->GetCamera();
    m_selectionFB->m_isPassing = true;
    m_selectionFB->PrepareForRender(scene);
    cam->SetReplacementShaderProgram(m_selectionFB->GetSelectionShaderProgram());

    m_selectionFB->Bind();

    m_selectionFB->ClearColor();
    m_sceneSelectionPass.Pass(renderers, sceneChildren);
    m_canvasSelectionPass.Pass(renderers, sceneChildren);
    m_gizmosSelectionPass.Pass(renderers, sceneChildren);

    m_selectionFB->UnBind();

    cam->SetReplacementShaderProgram(nullptr);
    m_selectionFB->ProcessSelection();
    m_selectionFB->m_isPassing = false;
    #endif
}

void GraphicPipeline::ApplySelectionEffect()
{
    #ifdef BANG_EDITOR

    if (!Hierarchy::GetInstance()->GetFirstSelectedGameObject()) { return; }

    List<GameObject*> sceneGameObjects =
            p_scene->GetChildrenRecursively();

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
    Camera *sceneCam = p_scene->GetCamera();
    if (rend)
    {
        renderRect = rend->gameObject->GetBoundingScreenRect(sceneCam, false);
    }
    else
    {
        // Apply deferred lights to the whole scene
        // TODO: fix getting bounding screen rect of scene InGame
        // renderRect = m_currentScene->GetBoundingScreenRect(sceneCam, true);
        renderRect = Rect::ScreenRect;
    }

    ASSERT(renderRect != Rect::Empty); // If the rect is empty, dont waste time rendering nothing

    m_gbuffer->SetStencilTest(true); // We have marked from before the zone where we want to apply the effect

    Material *rendMat = rend ? rend->GetMaterial() : nullptr;
    if ( !rend || (rendMat && rendMat->ReceivesLighting()) )
    {
        List<Light*> lights = p_scene->GetComponentsInChildren<Light>();
        for (Light *light : lights)
        {
            if (!CAN_USE_COMPONENT(light)) { continue; }
            light->ApplyLight(m_gbuffer, renderRect);
        }
    }
}

void GraphicPipeline::RenderPassWithMaterial(Material *mat,
                                             const Rect &renderRect)
{
    ASSERT(mat);
    ShaderProgram *sp = mat->GetShaderProgram(); ASSERT(sp);

    m_screenPlaneMesh->BindPositionsToShaderProgram(
      ShaderContract::Attr_Vertex_In_Position_Raw, *sp);

    sp->SetVec2("B_rectMinCoord", renderRect.GetMin());
    sp->SetVec2("B_rectMaxCoord", renderRect.GetMax());

    sp->OnRenderingStarts(nullptr, sp);
    mat->OnRenderingStarts(nullptr, sp);
    RenderScreenPlane();
    mat->OnRenderingEnds(nullptr, sp);
    sp->OnRenderingEnds(nullptr, sp);
}

void GraphicPipeline::RenderToScreen(Texture *fullScreenTexture)
{
    ShaderProgram *sp = m_renderGBufferToScreenMaterial->GetShaderProgram();
    ASSERT(sp);

    m_screenPlaneMesh->BindPositionsToShaderProgram(
                ShaderContract::Attr_Vertex_In_Position_Raw, *sp);

    sp->SetTexture("B_color_gout_fin", fullScreenTexture);

    sp->OnRenderingStarts(nullptr, sp);
    m_renderGBufferToScreenMaterial->OnRenderingStarts(nullptr, sp);
    GraphicPipeline::RenderScreenPlane();
    m_renderGBufferToScreenMaterial->OnRenderingEnds(nullptr, sp);
    sp->OnRenderingEnds(nullptr, sp);
}

void GraphicPipeline::RenderScreenPlane()
{
    m_screenPlaneMesh->GetVAO()->Bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthFunc(GL_ALWAYS);
    glDepthMask(GL_FALSE);
    glDrawArrays(GL_TRIANGLES, 0, m_screenPlaneMesh->GetVertexCount());
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    m_screenPlaneMesh->GetVAO()->UnBind();
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
        GBuffer::Attachment attachment)
{
    m_gbufferAttachToBeShown = attachment;
}

GLContext *GraphicPipeline::GetGLContext() const
{
    return m_glContext;
}

GBuffer *GraphicPipeline::GetGBuffer()
{
    return m_gbuffer;
}
