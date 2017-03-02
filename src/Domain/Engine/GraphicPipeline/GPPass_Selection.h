#ifndef GPPASS_SELECTION_H
#define GPPASS_SELECTION_H

#include "GPPass.h"

class SelectionFramebuffer;
class GPPass_Selection : public GPPass
{
public:
    GPPass_Selection(GraphicPipeline *graphicPipeline,
                     const List<GPPass*> &subPasses = {});

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

    virtual bool CanRender(const Renderer *renderer) const override;
};

#endif // GPPASS_SELECTION_H
