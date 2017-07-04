#ifndef GPPASS_SP_DEFERREDLIGHTS_H
#define GPPASS_SP_DEFERREDLIGHTS_H

#include "Bang/G_GPPass.h"

/**
 * @brief The GPDeferredLightsPass class is a GraphicPipeline pass
 * that applies the deferred lighting on every object currently present in
 * the G_GBuffer (marked in the stencil)
 */
class G_GPPass_SP_DeferredLights : public G_GPPass
{
public:
    G_GPPass_SP_DeferredLights(G_GraphicPipeline *graphicPipeline,
                             const List<G_GPPass*> &subPasses = {});

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;
};

#endif // GPPASS_SP_DEFERREDLIGHTS_H
