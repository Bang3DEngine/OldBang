#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

class Scene;
class GBuffer;
class Material;
/**
 * @brief The GraphicPipeline class is the responsible of rendering the scene.
 * These are the followed steps:
 * PART 1: OPAQUE. Everything rendered in this first part will be opaque.
 *                 We use deferred shading in this part.
 *      STEP 1: Render all opaque renderers in GameObjects.
 *      STEP 2: Render opaque Gizmos with depth.
 *
 * PART 2: TRANSPARENT. Render transparent stuff, using forward pipeline
 *                      in this case.
 *      STEP 1: Render all transparent renderers in GameObjects.
 *      STEP 2: Render transparent Gizmos with depth.
 *
 * PART 3: Some POST-RENDER EFFECTS.
 *      Apply PostRender effects, such as selection outline in editor.
 *
 * BREAK: This is a point in which we will clear the depth buffer.
 * We do this because we need to draw no-depth Gizmos now. If you think
 * it can be done without clearing the depth buffer, and merge below steps
 * with above steps, think that No-Depth Gizmos must respect the depth
 * between them when being drawn. I didn't find a better way to do this.
 *
 * PART 3: OPAQUE No-Depth Gizmos.
 *      STEP 1: Render opaque Gizmos using deferred shading.
 *
 * PART 4: TRANSPARENT No-Depth Gizmos.
 *      STEP 1: Render transparent Gizmos using forward shading.
 *
 *
 * FINAL: Write the GBuffer to the screen
 */
class GraphicPipeline
{
private:

    GBuffer *m_gbuffer = nullptr;

    Material *m_materialBeforeLighting = nullptr;
    Material *m_materialAfterLighting   = nullptr;

    // Indicates whether we are currently rendering opaque or transparent Renderers
    // Renderer's will use it to determine if they can render themselves or not
    bool m_opaquePass = false;

    /**
     * @brief PART 1. Renders the opaque Renderers, using Deferred rendering.
     */
    void RenderOpaque(Scene *scene);

    /**
     * @brief PART 2. Renders the transparent Renderers, using Forward rendering.
     * It renders them from furthest to closest.
     */
    void RenderTransparent(Scene *scene);

    /**
     * @brief PART 3. Some after effects, such as the selection one.
     */
    void RenderPostRenderEffects(Scene *scene);


    /**
     * @brief PART 4. Render no-depth opaque Gizmos. Using Deferred.
     */
    void RenderOpaqueNoDepthGizmos(Scene *scene);

    /**
     * @brief PART 5. Render no-depth transparent Gizmos. Using Forward.
     */
    void RenderTransparentNoDepthGizmos(Scene *scene);

    /**
     * @brief Apply al the scene lights over the current gbuffer.
     */
    void ApplyDeferredLights(Scene *scene);

public:
    GraphicPipeline();
    virtual ~GraphicPipeline();

    static GraphicPipeline* GetActive();

    void RenderScene(Scene *scene);

    void OnResize(int newWidth, int newHeight);

    bool IsInOpaquePass() const;
};

#endif // GRAPHICPIPELINE_H
