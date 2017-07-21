#ifndef GPPASS_G_GIZMOS_H
#define GPPASS_G_GIZMOS_H

#include "Bang/GPPass.h"
#include "Bang/GizmosPassType.h"

class GPPass_G_Gizmos : public GPPass
{
public:
    GPPass_G_Gizmos(GraphicPipeline *graphicPipeline,
                    GizmosPassType gizmosPassType,
                    const List<GPPass*> &subPasses = {});

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

private:
    GizmosPassType m_gizmosPassType;
};

#endif // GPPASS_G_GIZMOS_H
