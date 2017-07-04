#ifndef GPPASS_SELECTION_H
#define GPPASS_SELECTION_H

#include "Bang/G_GPPass.h"

class SelectionFramebuffer;
class GPPass_Selection : public G_GPPass
{
public:
    GPPass_Selection(G_GraphicPipeline *graphicPipeline,
                     const List<G_GPPass*> &subPasses = {});

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

    virtual bool CanRender(const Renderer *renderer) const override;
};

#endif // GPPASS_SELECTION_H
