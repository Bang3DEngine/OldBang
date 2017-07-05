#ifndef GPPASS_SP_DEFERREDLIGHTS_H
#define GPPASS_SP_DEFERREDLIGHTS_H

#include "Bang/GPPass.h"

/**
 * @brief The GPDeferredLightsPass class is a GraphicPipeline pass
 * that applies the deferred lighting on every object currently present in
 * the G_GBuffer (marked in the stencil)
 */
class GPPass_SP_DeferredLights : public GPPass
{
public:
    GPPass_SP_DeferredLights(GraphicPipeline *graphicPipeline,
                             const List<GPPass*> &subPasses = {});

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;
};

#endif // GPPASS_SP_DEFERREDLIGHTS_H
