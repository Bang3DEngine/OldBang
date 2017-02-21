#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

#include "GBuffer.h"
#include "Renderer.h"
#include "GPDepthLayerPass.h"

class Mesh;
class Scene;
class Screen;
class Texture;
class Material;
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
    void RenderCustomPR(Renderer *rend);
    void RenderToScreen(Texture *fullScreenTexture);
    void RenderPassWithMaterial(Material *mat,
                                const Rect &renderRect = Rect(-1, 1, -1, 1));
    void RenderScreenPlane();

    void OnResize(int newWidth, int newHeight);

    void ApplyPREffectToRenderer(const Renderer *renderer, Material *mat);

    void SetGBufferAttachmentToBeRendered(GBuffer::Attachment attachment);

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
    GBuffer *m_gbuffer = nullptr;
    #ifdef BANG_EDITOR
    SelectionFramebuffer *m_selectionFB = nullptr;
    #endif

    GPDepthLayerPass m_scenePass, m_canvasPass, m_gizmosPass;

    GBuffer::Attachment m_gbufferAttachmentToBeShown =
            GBuffer::Attachment::Color;

    // Rendering for the Game Screen or the Scene screen
    bool m_renderingInGame = false;

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    Scene *p_scene = nullptr;
    Renderer::DepthLayer m_currentDepthLayer =
                             Renderer::DepthLayer::DepthLayerScene;

    // For opaque
    Material *m_matSelectionEffectScreen = nullptr;

    void ApplySelectionEffect();

    void RenderGBuffer();

    #ifdef BANG_EDITOR
    void RenderSelectionFramebuffer();
    #endif

    void RenderPassWithDepthLayer(Renderer::DepthLayer depthLayer,
                                  Framebuffer *fb);
    void RenderGizmosPass(Framebuffer *fb);
};

#endif // GRAPHICPIPELINE_H
