#ifndef GPOPAQUEPASS_H
#define GPOPAQUEPASS_H

#include "GraphicPipelinePass.h"

class GBuffer;
class Renderer;
class GPOpaquePass : public GraphicPipelinePass
{
public:
    GPOpaquePass(GraphicPipeline *graphicPipeline);

    virtual void InPass(const List<Renderer*> &renderers) override;
    virtual void PostPass(const List<Renderer*> &renderers) override;

    virtual bool CanRender(const Renderer *renderer) const override;
};

#endif // GPOPAQUEPASS_H
