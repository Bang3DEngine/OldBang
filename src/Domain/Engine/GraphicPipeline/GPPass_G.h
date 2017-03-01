#ifndef GPPASS_G_H
#define GPPASS_G_H

#include "GraphicPipelinePass.h"

class GBuffer;
class Renderer;
/**
 * @brief The GPPass_G class is a GraphicPipeline pass that will
 * render all renderers into the GBuffer.
 */
class GPPass_G : public GraphicPipelinePass
{
public:
    GPPass_G(GraphicPipeline *graphicPipeline,
             bool receiveLighting,
             bool transparent);

    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren) override;

    virtual bool CanRender(const Renderer *renderer) const override;

private:
    bool m_receiveLighting = false;
    bool m_transparent = false;
};

#endif // GPPASS_G_H
