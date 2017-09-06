#include "Bang/UIRenderer.h"

USING_NAMESPACE_BANG

UIRenderer::UIRenderer()
{
    SetRenderPass(RenderPass::Canvas);
    SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
}

UIRenderer::~UIRenderer()
{
}
