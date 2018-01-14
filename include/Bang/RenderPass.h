#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

enum class RenderPass
{
    Scene              = 0,
    ScenePostProcess   = 1,

    Canvas             = 2,
    CanvasPostProcess  = 3,

    Overlay            = 4,
    OverlayPostProcess = 5
};

NAMESPACE_BANG_END

#endif // RENDERPASS_H
