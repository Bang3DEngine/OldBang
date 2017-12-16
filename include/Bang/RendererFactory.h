#ifndef RENDERERFACTORY_H
#define RENDERERFACTORY_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD class Renderer;

class RendererFactory
{
public:
    static void ConvertToGizmoRenderer(Renderer *r);

    RendererFactory() = delete;
    virtual ~RendererFactory() = delete;
};

NAMESPACE_BANG_END

#endif // RENDERERFACTORY_H

