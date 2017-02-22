#ifndef GPPASS_SELECTION_H
#define GPPASS_SELECTION_H

#include "GraphicPipelinePass.h"

class SelectionFramebuffer;
class GPPass_Selection : public GraphicPipelinePass
{
public:
    GPPass_Selection(GraphicPipeline *graphicPipeline);

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

    virtual bool CanRender(const Renderer *renderer) const override;
};

#endif // GPPASS_SELECTION_H
