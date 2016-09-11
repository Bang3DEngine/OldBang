#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

class Scene;
class GBuffer;
class Material;
class Renderer;
class GameObject;
class SelectionFramebuffer;
/**
 * @brief The GraphicPipeline class is the responsible of rendering the scene.
 */
class GraphicPipeline
{
private:

    GBuffer *m_gbuffer = nullptr;
    #ifdef BANG_EDITOR
    SelectionFramebuffer *m_selectionFB = nullptr;
    #endif

    // For opaque
    Material *m_matBeforeLightingScreen = nullptr;
    Material *m_matAfterLightingScreen  = nullptr;

    // For transparent, (local lighting, not screen)
    Material *m_matBeforeLightingMesh   = nullptr;
    Material *m_matAfterLightingMesh    = nullptr;

    bool m_transparentPass = false;

    void RenderOpaque(Scene *scene);
    void RenderTransparent(Scene *scene);
    void RenderNoDepth(Scene *scene);
    void RenderPostRenderEffects(Scene *scene);

    /**
     * @brief Apply all the scene lights over the current gbuffer.
     */
    void ApplyDeferredLightsToAllGBuffer(Scene *scene);

    /**
     * @brief Apply all the scene lights over the specified renderer.
     * @param scene
     */
    void ApplyDeferredLightsToRenderer(Scene *scene, Renderer *rend);

    void ApplyPREffectsToScreen(Scene *scene);

    #ifdef BANG_EDITOR
    void RenderSelectionFramebuffer(Scene *scene);
    #endif

public:
    GraphicPipeline();
    virtual ~GraphicPipeline();

    static GraphicPipeline* GetActive();

    void RenderScene(Scene *scene);

    void OnResize(int newWidth, int newHeight);

    /**
     * @brief Applies all the PR effects to a Renderer. This is called by
     * the transparent Gizmos, since the PR effects must be applied after
     * the rendering of each transparent renderer.
     */
    void ApplyPREffectsToRenderer(Renderer *renderer);

    #ifdef BANG_EDITOR
    SelectionFramebuffer* GetSelectionFramebuffer() const;
    #endif

    bool IsTransparentPass() const;
};

#endif // GRAPHICPIPELINE_H
