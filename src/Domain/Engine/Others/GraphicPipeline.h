#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

#include "Renderer.h"

class Scene;
class GBuffer;
class Material;
class GameObject;
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
        DepthLayerOrder [1] = {Renderer::DepthLayer::DepthLayerScene};/*,
                              Renderer::DepthLayer::DepthLayerCanvas,
                              Renderer::DepthLayer::DepthLayerGizmosOverlay};*/

    GBuffer *m_gbuffer = nullptr;
    #ifdef BANG_EDITOR
    SelectionFramebuffer *m_selectionFB = nullptr;
    #endif

    // For opaque
    Material *m_matAmbientLightScreen  = nullptr;
    Material *m_matEditorEffectsScreen = nullptr;

    void ApplyEditorEffects();

    /**
     * @brief Apply all the scene lights over the current gbuffer.
     */
    void ApplyDeferredLightsToScreen();

    #ifdef BANG_EDITOR
    void RenderSelectionFramebuffer(const List<Renderer*> &renderers);
    #endif

public:
    GraphicPipeline();
    virtual ~GraphicPipeline();

    static GraphicPipeline* GetActive();

    void RenderScene(Scene *scene);
    void RenderRenderer(Renderer *rend);

    void OnResize(int newWidth, int newHeight);

    void ApplyPREffectToRenderer(const Renderer *renderer, Material *mat);

    GBuffer *GetGBuffer() const;

    #ifdef BANG_EDITOR
    SelectionFramebuffer* GetSelectionFramebuffer() const;
    #endif
};

#endif // GRAPHICPIPELINE_H
