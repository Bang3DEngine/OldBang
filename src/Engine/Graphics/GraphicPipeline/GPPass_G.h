#ifndef GPPASS_G_H
#define GPPASS_G_H

#include "Bang/GPPass.h"

FORWARD class Renderer;

class G_GBuffer;
/**
 * @brief The GPPass_G class is a GraphicPipeline pass that will
 * render all renderers into the G_GBuffer.
 */
class GPPass_G : public GPPass
{
public:
    enum ReceiveLightPass { Yes, No, Both };

    GPPass_G(GraphicPipeline *graphicPipeline,
             ReceiveLightPass receiveLighting,
             const List<GPPass*> &subPasses = {});

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

    virtual bool CanRender(const Renderer *renderer) const override;

private:
    ReceiveLightPass m_receiveLighting;
};

#endif // GPPASS_G_H
