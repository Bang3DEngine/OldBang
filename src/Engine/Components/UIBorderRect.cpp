#include "Bang/UIBorderRect.h"

#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/ComponentFactory.h"

UIBorderRect::UIBorderRect()
{
    p_lineRenderer = ComponentFactory::CreateComponent<LineRenderer>();
    p_lineRenderer->SetPoints({});
    p_lineRenderer->UseMaterialCopy();
    p_lineRenderer->SetRenderPrimitive(GL::RenderPrimitive::LineStrip);

    // Work in Canvas and NDC space
    p_lineRenderer->SetRenderPass(RenderPass::Canvas);
    p_lineRenderer->SetViewProjMode(GL::ViewProjMode::IgnoreBoth);

    AddDelegate(p_lineRenderer);
    SetLineColor(Color::Black);
    SetLineWidth(5.0f);
}

UIBorderRect::~UIBorderRect()
{

}

void UIBorderRect::OnUpdate()
{
    Component::OnUpdate();
    ENSURE(gameObject);

    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);
    Rect boundingRectNDC = rt->GetScreenSpaceRectNDC();
    if (boundingRectNDC != m_latestBoundingRectNDC)
    {
        p_lineRenderer->SetPoints(
           {Vector3(boundingRectNDC.GetMinXMinY(), 0),
            Vector3(boundingRectNDC.GetMinXMaxY(), 0),
            Vector3(boundingRectNDC.GetMaxXMaxY(), 0),
            Vector3(boundingRectNDC.GetMaxXMinY(), 0),
            Vector3(boundingRectNDC.GetMinXMinY(), 0)} );
        m_latestBoundingRectNDC = boundingRectNDC;
    }
}

void UIBorderRect::SetLineColor(const Color &lineColor)
{
    p_lineRenderer->GetMaterial()->SetDiffuseColor(lineColor);
}
void UIBorderRect::SetLineWidth(float lineWidth)
{
    p_lineRenderer->SetLineWidth(lineWidth);
}

const Color &UIBorderRect::GetLineColor() const
{
    return p_lineRenderer->GetMaterial()->GetDiffuseColor();
}
float UIBorderRect::GetLineWidth() const
{
    return p_lineRenderer->GetLineWidth();
}
