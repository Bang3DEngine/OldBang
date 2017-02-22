#ifndef GPPASS_G_GIZMOS_H
#define GPPASS_G_GIZMOS_H

#include "GraphicPipelinePass.h"

class GPPass_G_Gizmos : public GraphicPipelinePass
{
public:
    GPPass_G_Gizmos(GraphicPipeline *graphicPipeline);

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

};

#endif // GPPASS_G_GIZMOS_H
