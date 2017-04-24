#ifndef GPPASS_SP_POSTPROCESSEFFECTS_H
#define GPPASS_SP_POSTPROCESSEFFECTS_H

#include "Bang/GPPass.h"
#include "Bang/PostProcessEffect.h"

class ShaderProgram;
class GPPass_SP_PostProcessEffects : public GPPass
{
public:
     GPPass_SP_PostProcessEffects(GraphicPipeline *graphicPipeline,
                                  PostProcessEffect::Type type,
                                  const List<GPPass*> &subPasses = {});

     virtual void InPass(const List<Renderer*> &renderers,
                         const List<GameObject*> &sceneChildren) override;

     PostProcessEffect::Type m_type;
};

#endif // GPPASS_SP_POSTPROCESSEFFECTS_H
