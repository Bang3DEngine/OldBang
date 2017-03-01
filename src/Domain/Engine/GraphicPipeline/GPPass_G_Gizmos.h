#ifndef GPPASS_G_GIZMOS_H
#define GPPASS_G_GIZMOS_H

#include "GraphicPipelinePass.h"

class GPPass_G_Gizmos : public GraphicPipelinePass
{
public:
    GPPass_G_Gizmos(GraphicPipeline *graphicPipeline,
                    bool depthed, bool overlay);

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

private:
    bool m_depthed = false;
    bool m_overlay = false;
};

#endif // GPPASS_G_GIZMOS_H
