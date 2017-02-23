#ifndef GPPASS_DEPTH_LAYER_H
#define GPPASS_DEPTH_LAYER_H

#include "Renderer.h"
#include "GraphicPipelinePass.h"

/**
 * @brief The GPDepthLayerPass class is a GraphicPipeline pass that does nothing
 * but filtering the renderers that belong to the specified depth layer.
 */
class GPPass_DepthLayer : public GraphicPipelinePass
{
public:
    GPPass_DepthLayer(GraphicPipeline *graphicPipeline,
                     Renderer::DepthLayer depthLayer);

    virtual void PrePass(const List<Renderer*> &renderers,
                         const List<GameObject*> &sceneChildren) override;

    virtual bool CanRender(const Renderer *renderer) const override;

    Renderer::DepthLayer GetDepthLayer() const;

protected:
    const Renderer::DepthLayer m_depthLayer;
};

#endif // GPPASS_DEPTH_LAYER_H
