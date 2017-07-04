#ifndef GPPASS_G_GIZMOS_H
#define GPPASS_G_GIZMOS_H

#include "Bang/G_GPPass.h"

class G_GPPass_G_Gizmos : public G_GPPass
{
public:
    G_GPPass_G_Gizmos(G_GraphicPipeline *graphicPipeline,
                    bool depthed, bool overlay,
                    const List<G_GPPass*> &subPasses = {});

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

private:
    bool m_depthed = false;
    bool m_overlay = false;
};

#endif // GPPASS_G_GIZMOS_H
