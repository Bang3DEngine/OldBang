#ifndef GPPASS_SP_FXAA_H
#define GPPASS_SP_FXAA_H

#include "Bang/GPPass.h"

class ShaderProgram;
class GPPass_SP_FXAA : public GPPass
{
public:
    GPPass_SP_FXAA(GraphicPipeline *graphicPipeline,
                   const List<GPPass*> &subPasses = {});

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

private:
    ShaderProgram *m_fxaaSP = nullptr;
};

#endif // GPPASS_SP_FXAA_H
