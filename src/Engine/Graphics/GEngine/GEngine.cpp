#include "Bang/GEngine.h"

#include "Bang/GL.h"
#include "Bang/VAO.h"
#include "Bang/VBO.h"
#include "Bang/Mesh.h"
#include "Bang/Scene.h"
#include "Bang/Light.h"
#include "Bang/Input.h"
#include "Bang/Window.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/ChronoGL.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/GLUniforms.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutManager.h"
#include "Bang/MaterialFactory.h"
#include "Bang/TextureUnitManager.h"
#include "Bang/SelectionFramebuffer.h"

USING_NAMESPACE_BANG

GEngine* GEngine::s_gEngine = nullptr;

GEngine::GEngine()
{
}

GEngine::~GEngine()
{
    delete m_texUnitManager;
    delete m_gl;
}

void GEngine::Init()
{
    m_gl = new GL();
    GL::SetActive( GetGL() );
    m_texUnitManager = new TextureUnitManager();

    p_screenPlaneMesh = Resources::Clone<Mesh>(MeshFactory::GetUIPlane());
    p_renderGBufferToScreenMaterial = MaterialFactory::GetRenderGBufferToScreen();
    GL::SetActive( nullptr );
}

void GEngine::Render(GameObject *go, Camera *camera)
{
    SetActiveCamera(camera);
    RenderToGBuffer(go, camera);
    RenderToSelectionFramebuffer(go, camera);
    SetActiveCamera(nullptr);
}

void GEngine::Render(Scene *scene)
{
    scene->BeforeRender();
    List<Camera*> sceneCameras = scene->GetComponentsInChildren<Camera>();
    for (Camera *camera : sceneCameras)
    {
        GameObject *gameObjectToRender = camera->GetGameObjectToRender();
        if (gameObjectToRender)
        {
            Render(gameObjectToRender, camera);
        }
    }
}

void GEngine::ApplyDeferredLights(GameObject *lightsContainer,
                                  Renderer *lightReceiver,
                                  Camera *camera)
{
    // Rect maskRect = lightReceiver->GetGameObject()->GetBoundingScreenRect(camera, false);
    // ENSURE(maskRect != Rect::Zero);
    ApplyDeferredLightsToGBuffer(lightsContainer, camera, Rect::NDCRect);
}

void GEngine::ApplyDeferredLights(GameObject *lightsContainer,
                                  GameObject *lightReceiver,
                                  Camera *camera)
{
    // Rect maskRect = lightReceiver->GetBoundingScreenRect(camera, true);
    // ENSURE(maskRect != Rect::Zero);
    ApplyDeferredLightsToGBuffer(lightsContainer, camera, Rect::NDCRect);
}

void GEngine::ApplyDeferredLightsToGBuffer(GameObject *lightsContainer,
                                           Camera *camera,
                                           const Rect &maskRectNDC)
{
    // We have marked from before the zone where we want to apply the effect
    GL::SetStencilValue(1);
    GL_Function latestStencilFunc = GL::GetStencilFunc();
    GL::SetStencilFunc(GL_Function::Equal);

    List<Light*> lights = lightsContainer->GetComponentsInChildren<Light>();
    for (Light *light : lights)
    {
        if (!light || !light->IsEnabled(true)) { continue; }
        light->ApplyLight(camera->GetGBuffer(), maskRectNDC);
    }

    GL::SetStencilFunc(latestStencilFunc);
}

void GEngine::Resize(int newWidth, int newHeight)
{
}

Camera *GEngine::GetActiveCamera()
{
    GEngine *ge = GEngine::GetActive();
    return ge ? ge->p_activeCamera : nullptr;
}

GBuffer *GEngine::GetActiveGBuffer()
{
    Camera *cam = GEngine::GetActiveCamera();
    return cam ? cam->GetGBuffer() : nullptr;
}

SelectionFramebuffer *GEngine::GetActiveSelectionFramebuffer()
{
    Camera *cam = GEngine::GetActiveCamera();
    return cam ? cam->GetSelectionFramebuffer() : nullptr;
}

void GEngine::RenderToGBuffer(GameObject *go, Camera *camera)
{
    camera->BindGBuffer();

    // GBuffer Scene rendering
    GL::Enablei(GL_Test::Blend, 3);
    GL::SetDepthMask(true); // Write depth
    GL::SetDepthFunc(GL_Function::LEqual);
    GL::SetStencilValue(1);
    GL::SetStencilOp(GL_StencilOperation::Replace); // Write to stencil
    go->Render(RenderPass::Scene_Lighted);

    // Apply lights to stenciled zone
    GL::SetStencilOp(GL_StencilOperation::Keep); // Dont modify stencil
    ApplyDeferredLights(go, go, camera);
    GL::SetStencilFunc(GL_Function::Always);
    GL::SetStencilValue(0);

    go->Render(RenderPass::Scene_UnLighted);
    go->Render(RenderPass::Scene_PostProcess);

    GL::Enable(GL_Test::Blend);
    GL::BlendFunc(GL_BlendFactor::SrcAlpha, GL_BlendFactor::OneMinusSrcAlpha);
    camera->GetGBuffer()->SetColorDrawBuffer();

    // GBuffer Canvas rendering
    GL::ClearStencilBuffer();
    GL::ClearDepthBuffer();
    GL::SetDepthMask(true);
    GL::SetDepthFunc(GL_Function::LEqual);
    go->Render(RenderPass::Canvas);
    go->Render(RenderPass::Canvas_PostProcess);

    // GBuffer Gizmos rendering
    GL::ClearStencilBuffer();
    GL::ClearDepthBuffer();
    GL::SetDepthMask(true);
    GL::SetStencilFunc(GL_Function::Always);
    GL::SetDepthFunc(GL_Function::LEqual);
    go->Render(RenderPass::Gizmos);
    go->RenderGizmos();

    GL::Disable(GL_Test::Blend);
}

void GEngine::RenderToSelectionFramebuffer(GameObject *go, Camera *camera)
{
    camera->BindSelectionFramebuffer();

    // Selection rendering
    camera->GetSelectionFramebuffer()->PrepareForRender(go);
    go->Render(RenderPass::Scene_Lighted);
    go->Render(RenderPass::Scene_UnLighted);
    GL::ClearStencilBuffer();
    GL::ClearDepthBuffer();
    // go->Render(RenderPass::Canvas);
    go->Render(RenderPass::Gizmos);
    go->RenderGizmos();
}

void GEngine::SetActive(GEngine *gEngine)
{
    GEngine::s_gEngine = gEngine;
    GL::SetActive( gEngine ? gEngine->GetGL() : nullptr );
}


void GEngine::ApplyScreenPass(ShaderProgram *sp, const Rect &mask)
{
    sp->Bind();
    sp->Set("B_rectMinCoord", mask.GetMin());
    sp->Set("B_rectMaxCoord", mask.GetMax());
    RenderScreenPlane();
    sp->UnBind();
}

void GEngine::RenderToScreen(Camera *cam)
{
    ENSURE(cam);
    p_renderGBufferToScreenMaterial.Get()->Bind();

    ShaderProgram *sp = p_renderGBufferToScreenMaterial.Get()->GetShaderProgram();
    GBuffer *gbuffer = cam->GetGBuffer();
    gbuffer->PrepareForRender(sp);
    sp->Set("B_GTex_Color", gbuffer->GetAttachmentTexture(GBuffer::AttColor));

    GEngine::RenderScreenPlane(true);

    p_renderGBufferToScreenMaterial.Get()->UnBind();
}

void GEngine::RenderToScreen(Texture2D *fullScreenTexture)
{
    ASSERT(fullScreenTexture);
    p_renderGBufferToScreenMaterial.Get()->Bind();

    ShaderProgram *sp = p_renderGBufferToScreenMaterial.Get()-> GetShaderProgram();
    sp->Set("B_GTex_Color", fullScreenTexture);

    GEngine::RenderScreenPlane();

    p_renderGBufferToScreenMaterial.Get()->UnBind();
}

void GEngine::RenderScreenPlane(bool withDepth)
{
    bool prevWireframe = GL::IsWireframe();
    GL::SetWireframe(false);

    GL::SetViewProjMode(GL_ViewProjMode::IgnoreBothAndModel);

    bool prevDepthMask = GL::GetDepthMask();
    GL_Function prevDepthFunc = GL::GetDepthFunc();

    if (!withDepth)
    {
        GL::SetDepthFunc(GL_Function::Always);
        GL::SetDepthMask(false);
    }

    GL::Render(p_screenPlaneMesh.Get()->GetVAO(), GL_Primitive::Triangles,
               p_screenPlaneMesh.Get()->GetVertexCount());

    GL::SetDepthMask(prevDepthMask);
    GL::SetDepthFunc(prevDepthFunc);
    GL::SetWireframe(prevWireframe);
}

GEngine* GEngine::GetActive()
{
    Window *win = Window::GetActive();
    return win ? win->GetGEngine() : nullptr;
}

void GEngine::SetActiveCamera(Camera *camera)
{
    if (p_activeCamera) { p_activeCamera->UnBind(); }

    p_activeCamera = camera;
    if (p_activeCamera) { p_activeCamera->Bind(); }
}

void GEngine::Render(Renderer *rend)
{
    Camera *activeCamera = p_activeCamera; ENSURE(activeCamera);

    if (GL::IsBound(activeCamera->GetSelectionFramebuffer()))
    {
        activeCamera->GetSelectionFramebuffer()->RenderForSelectionBuffer(rend);
    }
    else
    {
        ASSERT( GL::IsBound(activeCamera->GetGBuffer()) ||
                GL::GetBoundId(GL_BindTarget::DrawFramebuffer) > 0 );

        rend->Bind();

        Material *rendMat = rend->GetUserMaterial();
        if (rendMat)
        {
            activeCamera->GetGBuffer()->PrepareForRender(rendMat->GetShaderProgram());
            rend->OnRender();
            rend->UnBind();
        }
    }
}

GL *GEngine::GetGL() const { return m_gl; }
TextureUnitManager *GEngine::GetTextureUnitManager() const
{
    return m_texUnitManager;
}
