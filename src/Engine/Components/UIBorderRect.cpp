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

    // Work in NDC space
    p_lineRenderer->SetViewProjMode(GL::ViewProjMode::IgnoreBoth);

    SetLineWidth(2.0f);
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
            Vector3(boundingRectNDC.GetMaxXMinY(), 0)} );
        m_latestBoundingRectNDC = boundingRectNDC;
    }
}

void UIBorderRect::SetLineColor(const Color &lineColor)
{
    p_lineRenderer->GetMaterial()->SetDiffuseColor(lineColor);
}

const Color &UIBorderRect::GetLineColor() const
{
    return p_lineRenderer->GetMaterial()->GetDiffuseColor();
}

void UIBorderRect::SetLineWidth(float lineWidth)
{
    p_lineRenderer->SetLineWidth(lineWidth);
}

float UIBorderRect::GetLineWidth() const
{
    return p_lineRenderer->GetLineWidth();
}
