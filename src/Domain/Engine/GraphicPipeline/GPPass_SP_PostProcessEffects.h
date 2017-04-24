#ifndef GPPASS_SP_POSTPROCESSEFFECTS_H
#define GPPASS_SP_POSTPROCESSEFFECTS_H

#include "Bang/GPPass.h"

class ShaderProgram;
class GPPass_SP_PostProcessEffects : public GPPass
{
public:
     GPPass_SP_PostProcessEffects(GraphicPipeline *graphicPipeline,
                                  const List<GPPass*> &subPasses = {});

     virtual void InPass(const List<Renderer*> &renderers,
                         const List<GameObject*> &sceneChildren) override;
};

#endif // GPPASS_SP_POSTPROCESSEFFECTS_H
