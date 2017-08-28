#include "Bang/UIRenderer.h"

UIRenderer::UIRenderer()
{
    SetRenderPass(RenderPass::Canvas);
    SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
}

UIRenderer::~UIRenderer()
{
}
