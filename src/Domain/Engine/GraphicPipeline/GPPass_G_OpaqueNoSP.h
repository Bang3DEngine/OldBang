#ifndef GPPASS_G_OPAQUENOSP_H
#define GPPASS_G_OPAQUENOSP_H

#include "GraphicPipelinePass.h"

/**
 * @brief The GPOpaqueLightedPass class is a GraphicPipeline pass that draws
 * very simple opaque objects. This is, it draws renderers which won't need any
 * ScreenPass after it. So we will not stencil them into the GBuffer.
 */
class GBuffer;
class Renderer;
class GPPass_G_OpaqueNoSP : public GraphicPipelinePass
{
public:
    GPPass_G_OpaqueNoSP(GraphicPipeline *graphicPipeline);

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

    virtual bool CanRender(const Renderer *renderer) const override;
};

#endif // GPPASS_G_OPAQUENOSP_H
