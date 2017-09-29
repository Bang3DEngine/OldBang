#include "Bang/UIBorderRect.h"

#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/ComponentFactory.h"

USING_NAMESPACE_BANG

UIBorderRect::UIBorderRect()
{
    p_lineRenderer = ComponentFactory::CreateComponent<LineRenderer>();
    p_lineRenderer->SetPoints({});
    p_lineRenderer->UseMaterialCopy();
    p_lineRenderer->SetRenderPrimitive(GL::Primitives::LineStrip);

    // Work in Canvas and NDC space
    SetRenderPass(RenderPass::Canvas);
    p_lineRenderer->SetRenderPass(RenderPass::Canvas);
    p_lineRenderer->SetViewProjMode(GL::ViewProjMode::IgnoreBoth);

    SetLineColor(Color::Black);
    SetLineWidth(1.0f);
}

UIBorderRect::~UIBorderRect()
{

}

void UIBorderRect::OnStart()
{
    Component::OnStart();
    p_lineRenderer->OnStart();
}

void UIBorderRect::OnRender()
{
    Vector3 translate(0, 0, gameObject->transform->GetPosition().z);
    GL::SetModelMatrix( Matrix4::TranslateMatrix(translate) );
    gameObject->transform->SetEnabled(false);
    p_lineRenderer->Renderer::OnRender(RenderPass::Canvas);
    gameObject->transform->SetEnabled(true);
}


void UIBorderRect::SetLineColor(const Color &lineColor)
{
    p_lineRenderer->GetMaterial()->SetDiffuseColor(lineColor);
}
void UIBorderRect::SetLineWidth(float lineWidth)
{
    p_lineRenderer->SetLineWidth(lineWidth);
}

void UIBorderRect::SetPadding(int paddingPx)
{
    m_padding = paddingPx;
}

const Color &UIBorderRect::GetLineColor() const
{
    return p_lineRenderer->GetMaterial()->GetDiffuseColor();
}
float UIBorderRect::GetLineWidth() const
{
    return p_lineRenderer->GetLineWidth();
}

int UIBorderRect::GetPaddingPx() const
{
    return m_padding;
}

void UIBorderRect::OnRectTransformChanged()
{
    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);
    Rect boundingRectNDC = rt->GetScreenSpaceRectNDC();

    Vector2 pNDC = GL::FromPixelsAmountToGlobalNDC( Vector2i(m_padding) );
    p_lineRenderer->SetPoints(
      {Vector3(boundingRectNDC.GetMinXMinY() + Vector2( pNDC.x,  pNDC.y), 0),
       Vector3(boundingRectNDC.GetMinXMaxY() + Vector2( pNDC.x, -pNDC.y), 0),
       Vector3(boundingRectNDC.GetMaxXMaxY() + Vector2(-pNDC.x, -pNDC.y), 0),
       Vector3(boundingRectNDC.GetMaxXMinY() + Vector2(-pNDC.x,  pNDC.y), 0),
       Vector3(boundingRectNDC.GetMinXMinY() + Vector2( pNDC.x,  pNDC.y), 0),} );
}

void UIBorderRect::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("LineColor"))
    { SetLineColor( xmlInfo.Get<Color>("LineColor") ); }
}

void UIBorderRect::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("LineColor", GetLineColor());
}
