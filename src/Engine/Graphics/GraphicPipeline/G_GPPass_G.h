#ifndef GPPASS_G_H
#define GPPASS_G_H

#include "Bang/G_GPPass.h"

class Renderer;
class G_GBuffer;
/**
 * @brief The GPPass_G class is a GraphicPipeline pass that will
 * render all renderers into the G_GBuffer.
 */
class G_GPPass_G : public G_GPPass
{
public:
    G_GPPass_G(G_GraphicPipeline *graphicPipeline,
             bool receiveLighting, bool transparentPass,
             const List<G_GPPass*> &subPasses = {});

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

    virtual bool CanRender(const Renderer *renderer) const override;

private:
    bool m_receiveLighting = false;
    bool m_transparentPass = false;
};

#endif // GPPASS_G_H
