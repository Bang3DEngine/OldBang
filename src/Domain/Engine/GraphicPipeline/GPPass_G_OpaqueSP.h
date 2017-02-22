#ifndef GPPASS_G_OPAQUESP_H
#define GPPASS_G_OPAQUESP_H

#include "GraphicPipelinePass.h"

class GBuffer;
class Renderer;
/**
 * @brief The GPOpaqueLightedPass class is a GraphicPipeline pass that will
 * render all the opaque renderers that later will need some sort of ScreenPass,
 * such as deferred lighting. Because of this, it will mark the stencil with
 * every drawn renderer.
 */
class GPPass_G_OpaqueSP : public GraphicPipelinePass
{
public:
    GPPass_G_OpaqueSP(GraphicPipeline *graphicPipeline);

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

    virtual bool CanRender(const Renderer *renderer) const override;
};

#endif // GPPASS_G_OPAQUESP_H
