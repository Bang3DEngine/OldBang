#ifndef RENDERPASS_H
#define RENDERPASS_H

enum class RenderPass
{
    Scene_Lighted,
    Scene_UnLighted,
    Scene_PostProcess,

    Canvas,
    Canvas_PostProcess,

    Gizmos,

    Scene_Selection,
    Canvas_Selection
};

#endif // RENDERPASS_H
