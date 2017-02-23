#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

#include "GBuffer.h"
#include "Renderer.h"
#include "GPPass_DepthLayer.h"

class Mesh;
class Scene;
class Screen;
class Texture;
class Material;
class GLContext;
class GameObject;
class Framebuffer;
class SelectionFramebuffer;
/**
 * @brief The GraphicPipeline class is the responsible of rendering the scene.
 */
class GraphicPipeline
{
public:
    GraphicPipeline(Screen *screen);
    virtual ~GraphicPipeline();

    static GraphicPipeline* GetActive();

    void RenderScene(Scene *scene, bool inGame);
    void RenderRenderer(Renderer *rend);
    void RenderToScreen(Texture *fullScreenTexture);
    void RenderPassWithMaterial(Material *mat,
                                const Rect &renderRect = Rect(-1, 1, -1, 1));
    void RenderScreenPlane();

    void OnResize(int newWidth, int newHeight);

    void ApplySPEffectToRenderer(const Renderer *renderer, Material *mat);

    void SetGBufferAttachmentToBeRendered(GBuffer::Attachment attachment);

    GLContext *GetGLContext() const;

    GBuffer *GetGBuffer();
    Renderer::DepthLayer GetCurrentDepthLayer() const;

    #ifdef BANG_EDITOR
    SelectionFramebuffer* GetSelectionFramebuffer();
    #endif

    /**
     * @brief Apply all the scene lights over the current gbuffer.
     */
    void ApplyDeferredLights(Renderer *rend = nullptr);

private:
    GLContext *m_glContext = nullptr;

    GBuffer *m_gbuffer = nullptr;
    #ifdef BANG_EDITOR
    SelectionFramebuffer *m_selectionFB = nullptr;
    #endif

    GPPass_DepthLayer m_scenePass, m_canvasPass, m_gizmosPass;

    #ifdef BANG_EDITOR
    GPPass_DepthLayer m_sceneSelectionPass, m_canvasSelectionPass,
                      m_gizmosSelectionPass;
    #endif

    GBuffer::Attachment m_gbufferAttachToBeShown = GBuffer::Attachment::Color;

    // Rendering for the Game Screen or the Scene screen
    bool m_renderingInGame = false;

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    Scene *p_scene = nullptr;

    Material *m_matSelectionEffectScreen = nullptr;

    void ApplySelectionEffect();
};

#endif // GRAPHICPIPELINE_H
