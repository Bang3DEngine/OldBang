#ifndef GPPASS_SP_DEFERREDLIGHTS_H
#define GPPASS_SP_DEFERREDLIGHTS_H

#include "GraphicPipelinePass.h"

/**
 * @brief The GPDeferredLightsPass class is a GraphicPipeline pass
 * that applies the deferred lighting on every object currently present in
 * the GBuffer (marked in the stencil)
 */
class GPPass_SP_DeferredLights : public GraphicPipelinePass
{
public:
    GPPass_SP_DeferredLights(GraphicPipeline *graphicPipeline);

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;
};

#endif // GPPASS_SP_DEFERREDLIGHTS_H
