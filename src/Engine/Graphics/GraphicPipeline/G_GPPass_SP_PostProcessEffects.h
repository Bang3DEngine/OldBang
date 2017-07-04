#ifndef GPPASS_SP_POSTPROCESSEFFECTS_H
#define GPPASS_SP_POSTPROCESSEFFECTS_H

#include "Bang/G_GPPass.h"
#include "Bang/PostProcessEffect.h"

class G_ShaderProgram;
class G_GPPass_SP_PostProcessEffects : public G_GPPass
{
public:
     G_GPPass_SP_PostProcessEffects(G_GraphicPipeline *graphicPipeline,
                                  PostProcessEffect::Type type,
                                  const List<G_GPPass*> &subPasses = {});

     virtual void InPass(const List<Renderer*> &renderers,
                         const List<GameObject*> &sceneChildren) override;

     PostProcessEffect::Type m_type;
};

#endif // GPPASS_SP_POSTPROCESSEFFECTS_H
