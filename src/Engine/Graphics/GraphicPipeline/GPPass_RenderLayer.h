#ifndef GPPASS_DEPTH_LAYER_H
#define GPPASS_DEPTH_LAYER_H

#include "Bang/GPPass.h"
#include "Bang/Renderer.h"

/**
 * @briefGraphicPipeline pass that does nothing
 * but filtering the renderers that belong to the specified layer.
 */
class GPPass_RenderLayer : public GPPass
{
public:
    GPPass_RenderLayer(GraphicPipeline *graphicPipeline,
                      Renderer::RenderLayer depthLayer,
                      const List<GPPass*> &subPasses = {});

    virtual bool CanRender(const Renderer *renderer) const override;

    Renderer::RenderLayer GetRenderLayer() const;

protected:
    Renderer::RenderLayer m_renderLayer;
};

#endif // GPPASS_DEPTH_LAYER_H
