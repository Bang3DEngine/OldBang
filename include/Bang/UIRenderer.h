#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "Bang/Color.h"
#include "Bang/Renderer.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

class UIRenderer : public Renderer
{
    COMPONENT(UIRenderer)

protected:
    UIRenderer();
    virtual ~UIRenderer();

    virtual Rect GetBoundingRect(Camera *camera) const override;
};

NAMESPACE_BANG_END

#endif // UIRENDERER_H
