#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

class Scene;
class GBuffer;
class Material;
class Renderer;
class GameObject;
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

    // For opaque
    Material *m_matBeforeLightingScreen = nullptr;
    Material *m_matAfterLightingScreen  = nullptr;

    // For transparent, (local lighting, not screen)
    Material *m_matBeforeLightingMesh   = nullptr;
    Material *m_matAfterLightingMesh    = nullptr;

    /**
     * @brief PART 3. Some after effects, such as the selection one.
     */
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

public:
    GraphicPipeline();
    virtual ~GraphicPipeline();

    static GraphicPipeline* GetActive();

    void RenderScene(Scene *scene);

    void OnResize(int newWidth, int newHeight);
};

#endif // GRAPHICPIPELINE_H
