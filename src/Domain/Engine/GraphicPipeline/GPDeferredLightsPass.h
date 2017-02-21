#ifndef GPDEFERREDLIGHTSPASS_H
#define GPDEFERREDLIGHTSPASS_H

#include "GraphicPipelinePass.h"

class GPDeferredLightsPass : public GraphicPipelinePass
{
public:
    GPDeferredLightsPass(GraphicPipeline *graphicPipeline);

    virtual void InPass(const List<Renderer*> &renderers) override;
};

#endif // GPDEFERREDLIGHTSPASS_H
