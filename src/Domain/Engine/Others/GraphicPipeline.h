#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

#include "Renderer.h"

class Scene;
class Screen;
class GBuffer;
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

private:

    Scene *m_currentScene = nullptr;
    Renderer::DepthLayer m_currentDepthLayer =
            Renderer::DepthLayer::DepthLayerScene;
    const Renderer::DepthLayer
        DepthLayerOrder [3] = {Renderer::DepthLayer::DepthLayerScene,
                               Renderer::DepthLayer::DepthLayerCanvas,
                               Renderer::DepthLayer::DepthLayerGizmosOverlay};

    GBuffer *m_gbuffer = nullptr;
    #ifdef BANG_EDITOR
    SelectionFramebuffer *m_selectionFB = nullptr;
    #endif

    // For opaque
    Material *m_matAmbientLightScreen    = nullptr;
    Material *m_matSelectionEffectScreen = nullptr;

    void ApplySelectionEffect();

    /**
     * @brief Apply all the scene lights over the current gbuffer.
     */
    void ApplyDeferredLights(Renderer *rend = nullptr);
    void ApplyDeferredLightsToRenderer();

    #ifdef BANG_EDITOR
    void RenderSelectionFramebuffer();
    #endif

    void RenderPassWithDepthLayer(Renderer::DepthLayer depthLayer,
                                  Framebuffer *fb);
    void RenderGizmosOverlayPass(Framebuffer *fb);
    void RenderDepthLayers(Framebuffer *fb);

public:
    GraphicPipeline(Screen *screen);
    virtual ~GraphicPipeline();

    static GraphicPipeline* GetActive();

    void RenderScene(Scene *scene);
    void RenderRenderer(Renderer *rend);
    void RenderCustomPR(Renderer *rend);

    void OnResize(int newWidth, int newHeight);

    void ApplyPREffectToRenderer(const Renderer *renderer, Material *mat);

    GBuffer *GetGBuffer() const;
    Renderer::DepthLayer GetCurrentDepthLayer() const;

    #ifdef BANG_EDITOR
    SelectionFramebuffer* GetSelectionFramebuffer() const;
    #endif
};

#endif // GRAPHICPIPELINE_H
