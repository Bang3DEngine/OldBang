#include "Bang/UIRenderer.h"

#include "Bang/Rect.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

UIRenderer::UIRenderer()
{
    SetRenderPass(RenderPass::Canvas);
    SetViewProjMode(GL::ViewProjMode::IgnoreBoth);

    SetCullByRectTransform(true);
}

UIRenderer::~UIRenderer()
{
}

void UIRenderer::OnRender()
{
    Renderer::OnRender();
}

void UIRenderer::OnRender(RenderPass renderPass)
{
    bool render = true;
    if (GetCullByRectTransform())
    {
        RectTransform *rt = GetGameObject()->GetRectTransform();
        if (rt)
        {
            if (Rect::Intersection(Rect::ScreenRectNDC,
                                   rt->GetScreenRectNDC()) == Rect::Zero)
            {
                render = false;
            }
        }
    }

    if (render)
    {
        Renderer::OnRender(renderPass);
    }
}

void UIRenderer::SetCullByRectTransform(bool cullByRectTransform)
{
    m_cullByRectTransform = cullByRectTransform;
}

bool UIRenderer::GetCullByRectTransform() const
{
    return m_cullByRectTransform;
}

Rect UIRenderer::GetBoundingRect(Camera *camera) const
{
    GameObject *go = GetGameObject();
    RectTransform *rt = go ? go->GetRectTransform() : nullptr;
    return rt ? rt->GetScreenRectNDC() : Rect::Zero;
}
