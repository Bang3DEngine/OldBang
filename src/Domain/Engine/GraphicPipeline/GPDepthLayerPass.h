#ifndef GPDEPTHLAYERPASS_H
#define GPDEPTHLAYERPASS_H

#include "Renderer.h"
#include "GraphicPipelinePass.h"

class GPDepthLayerPass : public GraphicPipelinePass
{
public:
    GPDepthLayerPass(GraphicPipeline *graphicPipeline,
                     Renderer::DepthLayer depthLayer);

    virtual void PrePass(const List<Renderer*> &renderers) override;

    virtual bool CanRender(const Renderer *renderer) const override;

protected:
    const Renderer::DepthLayer m_depthLayer;
};

#endif // GPDEPTHLAYERPASS_H
