#ifndef RENDERPASS_H
#define RENDERPASS_H

enum class RenderPass
{
    Scene_Lighted,
    Scene_UnLighted,
    Scene_PostProcess,

    Canvas,
    Canvas_PostProcess,

    Gizmos
};

#endif // RENDERPASS_H
