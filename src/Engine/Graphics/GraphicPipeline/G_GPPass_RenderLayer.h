#ifndef GPPASS_DEPTH_LAYER_H
#define GPPASS_DEPTH_LAYER_H

#include "Bang/Renderer.h"
#include "Bang/G_GPPass.h"

/**
 * @briefGraphicPipeline pass that does nothing
 * but filtering the renderers that belong to the specified layer.
 */
class G_GPPass_RenderLayer : public G_GPPass
{
public:
    G_GPPass_RenderLayer(G_GraphicPipeline *graphicPipeline,
                      Renderer::RenderLayer depthLayer,
                      const List<G_GPPass*> &subPasses = {});

    virtual bool CanRender(const Renderer *renderer) const override;

    Renderer::RenderLayer GetRenderLayer() const;

protected:
    Renderer::RenderLayer m_renderLayer;
};

#endif // GPPASS_DEPTH_LAYER_H
