#include "Bang/GraphicPipeline.h"

#include "Bang/Mesh.h"
#include "Bang/G_VAO.h"
#include "Bang/G_VBO.h"
#include "Bang/Scene.h"
#include "Bang/Light.h"
#include "Bang/Input.h"
#include "Bang/Screen.h"
#include "Bang/Camera.h"
#include "Bang/ChronoGL.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
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
#include "Bang/G_TextureUnitManager.h"
#include "Bang/SelectionFramebuffer.h"

GraphicPipeline::GraphicPipeline(G_Screen *screen)
{
    m_glContext = new GLContext();
    m_texUnitManager = new G_TextureUnitManager();

    m_gbuffer = new G_GBuffer(screen->GetWidth(),
                              screen->GetHeight());
    m_selectionFB = new SelectionFramebuffer(screen->GetWidth(),
                                             screen->GetHeight());

    m_renderGBufferToScreenMaterial =
         AssetsManager::Load<Material>(
                EPATH("Materials/RenderGBufferToScreen.bmat") );

    m_screenPlaneMesh = MeshFactory::GetUIPlane();
}

void GraphicPipeline::RenderScene(Scene *scene, bool inGame)
{
    p_scene = scene; ENSURE(p_scene);
    m_renderingInGame = inGame;

    Camera *camera = scene->GetCamera();
    if (camera) { camera->Bind(); }

    RenderGBuffer(scene);
    RenderToScreen(m_gbuffer->GetAttachmentTexture(G_GBuffer::AttColor));

    RenderSelectionBuffer(scene);
    // RenderToScreen(m_selectionFB->GetColorTexture());
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
    ENSURE(renderRect != Rect::Zero);

    // We have marked from before the zone where we want to apply the effect
    m_gbuffer->SetStencilTest(true);

    Material *rendMat = rend ? rend->GetMaterial() : nullptr;
    if ( !rend || (rendMat && rendMat->GetReceivesLighting()) )
    {
        List<Light*> lights = p_scene->GetComponentsInChildren<Light>();
        for (Light *light : lights)
        {
            if (!light || !light->IsEnabled()) { continue; }
            light->ApplyLight(m_gbuffer, renderRect);
        }
    }

    m_gbuffer->SetStencilTest(false);
}

void GraphicPipeline::RenderGBuffer(Scene *scene)
{
    m_gbuffer->Bind();
    Color bgColor = p_scene->GetCamera()->GetClearColor();
    m_gbuffer->ClearBuffersAndBackground(bgColor);
    m_gbuffer->SetAllDrawBuffers();

    // GBuffer Scene rendering
    m_gbuffer->SetStencilWrite(true);
    scene->Render(RenderPass::Scene_Lighted);
    m_gbuffer->SetStencilWrite(false);
    ApplyDeferredLights();
    scene->Render(RenderPass::Scene_UnLighted);
    scene->Render(RenderPass::Scene_PostProcess);
    m_gbuffer->ClearStencil();

    // GBuffer Canvas rendering
    m_gbuffer->SetAllDrawBuffers();
    GL::SetTestDepth(false);
    scene->Render(RenderPass::Canvas);
    scene->Render(RenderPass::Canvas_PostProcess);
    GL::SetTestDepth(true);

    // GBuffer Gizmos rendering
    scene->RenderGizmos();

    m_gbuffer->UnBind();
}
void GraphicPipeline::RenderSelectionBuffer(Scene *scene)
{
    m_selectionFB->Bind();
    m_selectionFB->ClearDepth();
    m_selectionFB->ClearColor(Color::One);

    // Selection rendering
    m_selectionFB->PrepareForRender(scene);
    scene->Render(RenderPass::Scene_Lighted);
    scene->Render(RenderPass::Scene_UnLighted);
    scene->Render(RenderPass::Canvas);
    scene->RenderGizmos();

    m_selectionFB->UnBind();
}


void GraphicPipeline::ApplyScreenPass(G_ShaderProgram *sp, const Rect &mask)
{
    sp->Bind();
    m_glContext->ApplyToShaderProgram(sp);
    sp->Set("B_rectMinCoord", mask.GetMin());
    sp->Set("B_rectMaxCoord", mask.GetMax());
    sp->Set("B_ScreenSize", Vector2f(Screen::GetSize()));
    RenderScreenPlane();
    sp->UnBind();
}

void GraphicPipeline::RenderToScreen(G_Texture *fullScreenTexture)
{
    m_renderGBufferToScreenMaterial->Bind();

    ShaderProgram *sp = m_renderGBufferToScreenMaterial->GetShaderProgram();
    m_gbuffer->BindTextureBuffersTo(sp, false);
    sp->Set("B_GTex_Color", fullScreenTexture);

    GraphicPipeline::RenderScreenPlane();

    m_renderGBufferToScreenMaterial->UnBind();
}

void GraphicPipeline::RenderScreenPlane()
{
    GL::SetWireframe(false);
    GL::SetTestDepth(false);
    GL::SetWriteDepth(false);
    GL::SetCullMode(GL::CullMode::None);
    GL::Render(m_screenPlaneMesh->GetVAO(), GL::RenderPrimitive::Triangles,
               m_screenPlaneMesh->GetVertexCount());
    GL::SetWriteDepth(true);
    GL::SetTestDepth(true);
}

GraphicPipeline* GraphicPipeline::GetActive()
{
    Screen *screen = Screen::GetInstance();
    return screen ? screen->GetGraphicPipeline() : nullptr;
}

void GraphicPipeline::OnResize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
    m_selectionFB->Resize(newWidth, newHeight);
}

void GraphicPipeline::Render(Renderer *rend)
{
    if (GL::IsBound(m_gbuffer))
    {
        rend->Bind();
        Material *rendMat = rend->GetMaterial();
        m_gbuffer->BindTextureBuffersTo(rendMat->GetShaderProgram(), false);
        rend->OnRender();
        rend->UnBind();
    }
    else if (GL::IsBound(m_selectionFB))
    {
        m_selectionFB->RenderForSelectionBuffer(rend);
    }
}

GLContext *GraphicPipeline::GetGLContext() const
{
    return m_glContext;
}

G_GBuffer *GraphicPipeline::GetGBuffer()
{
    return m_gbuffer;
}

SelectionFramebuffer *GraphicPipeline::GetSelectionFramebuffer()
{
    return m_selectionFB;
}

G_TextureUnitManager *GraphicPipeline::GetTextureUnitManager() const
{
    return m_texUnitManager;
}

GraphicPipeline::~GraphicPipeline()
{
    delete m_gbuffer;
    delete m_selectionFB;
    delete m_texUnitManager;
    delete m_glContext;
}
