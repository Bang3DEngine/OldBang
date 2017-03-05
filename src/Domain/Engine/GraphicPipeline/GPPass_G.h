#ifndef GPPASS_G_H
#define GPPASS_G_H

#include "GPPass.h"

class GBuffer;
class Renderer;
/**
 * @brief The GPPass_G class is a GraphicPipeline pass that will
 * render all renderers into the GBuffer.
 */
class GPPass_G : public GPPass
{
public:
    GPPass_G(GraphicPipeline *graphicPipeline,
             bool receiveLighting, bool transparentPass,
             const List<GPPass*> &subPasses = {});

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

    virtual bool CanRender(const Renderer *renderer) const override;

private:
    bool m_receiveLighting = false;
    bool m_transparentPass = false;
};

#endif // GPPASS_G_H
