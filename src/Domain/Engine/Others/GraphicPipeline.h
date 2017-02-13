#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

#include "GBuffer.h"
#include "Renderer.h"

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

    GBuffer *GetGBuffer() const;
    Renderer::DepthLayer GetCurrentDepthLayer() const;

    #ifdef BANG_EDITOR
    SelectionFramebuffer* GetSelectionFramebuffer() const;
    #endif

private:
    GBuffer::Attachment m_gbufferAttachmentToBeShown = GBuffer::Attachment::Color;

    bool m_renderingInGame = false; // Rendering for the Game Screen or the Scene screen

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    Scene *m_currentScene = nullptr;
    Renderer::DepthLayer m_currentDepthLayer =
                             Renderer::DepthLayer::DepthLayerScene;

    GBuffer *m_gbuffer = nullptr;
    #ifdef BANG_EDITOR
    SelectionFramebuffer *m_selectionFB = nullptr;
    #endif

    // For opaque
    Material *m_matSelectionEffectScreen = nullptr;

    void ApplySelectionEffect();

    /**
     * @brief Apply all the scene lights over the current gbuffer.
     */
    void ApplyDeferredLights(Renderer *rend = nullptr);
    void RenderGBuffer();

    #ifdef BANG_EDITOR
    void RenderSelectionFramebuffer();
    #endif

    void RenderPassWithDepthLayer(Renderer::DepthLayer depthLayer,
                                  Framebuffer *fb);
    void RenderGizmosPass(Framebuffer *fb);
};

#endif // GRAPHICPIPELINE_H
