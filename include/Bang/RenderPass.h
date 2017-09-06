#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

enum class RenderPass
{
    Scene_Lighted,
    Scene_UnLighted,
    Scene_PostProcess,

    Canvas,
    Canvas_PostProcess,

    Gizmos
};

NAMESPACE_BANG_END

#endif // RENDERPASS_H
