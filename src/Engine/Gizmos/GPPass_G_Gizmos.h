#ifndef GPPASS_G_GIZMOS_H
#define GPPASS_G_GIZMOS_H

#include "Bang/GPPass.h"

class GPPass_G_Gizmos : public GPPass
{
public:
    GPPass_G_Gizmos(GraphicPipeline *graphicPipeline,
                    const List<GPPass*> &subPasses = {});

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;
};

#endif // GPPASS_G_GIZMOS_H
