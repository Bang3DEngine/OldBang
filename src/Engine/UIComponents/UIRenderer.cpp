#include "Bang/UIRenderer.h"

#include "Bang/Rect.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

UIRenderer::UIRenderer()
{
    SetRenderPass(RenderPass::Canvas);
    SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
}

UIRenderer::~UIRenderer()
{
}

Rect UIRenderer::GetBoundingRect(Camera *camera) const
{
    GameObject *go = GetGameObject();
    RectTransform *rt = go ? go->GetComponent<RectTransform>() : nullptr;
    return rt ? rt->GetScreenSpaceRectNDC() : Rect::Zero;
}
