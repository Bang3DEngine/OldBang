#include "Bang/UIAspectRatioFitter.h"

#include "Bang/AARect.h"
#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

UIAspectRatioFitter::UIAspectRatioFitter()
{
    SetAspectRatioMode(AspectRatioMode::Keep);
}

UIAspectRatioFitter::~UIAspectRatioFitter()
{
}

void UIAspectRatioFitter::SetAspectRatio(float aspectRatio)
{
    if (aspectRatio != GetAspectRatio())
    {
        m_aspectRatio = aspectRatio;
        Invalidate();
    }
}

void UIAspectRatioFitter::SetAspectRatio(const Vector2 &size)
{
    SetAspectRatio( size.x / Math::Max(size.y, 1.0f) );
}

void UIAspectRatioFitter::SetAspectRatio(const Vector2i &size)
{
    SetAspectRatio( Vector2(size) );
}

float UIAspectRatioFitter::GetAspectRatio() const
{
    return m_aspectRatio;
}

void UIAspectRatioFitter::SetAspectRatioMode(AspectRatioMode arMode)
{
    if (arMode != GetAspectRatioMode())
    {
        m_aspectRatioMode = arMode;
        Invalidate();
    }
}

AspectRatioMode UIAspectRatioFitter::GetAspectRatioMode() const
{
    return m_aspectRatioMode;
}

void UIAspectRatioFitter::ApplyLayout(Axis axis)
{
    RectTransform *rt = GetGameObject()->GetRectTransform();
    if (!rt) { return; }

    GameObject *parent = GetGameObject()->GetParent();
    if (!parent) { return; }

    RectTransform *parentRT = parent->GetRectTransform();
    if (!parentRT) { return; }

    const Vector2i parentSize = Vector2i::Max(Vector2i::One,
                                   Vector2i(parentRT->GetViewportRect().GetSize()));

    Vector2i currentSize( Vector2::Round( Vector2(GetAspectRatio(), 1) * 30000.0f ) );
    Vector2i newSize = AspectRatio::GetAspectRatioedSize(currentSize,
                                                         parentSize,
                                                         GetAspectRatioMode());
    if (axis == Axis::Vertical)
    {
        rt->SetWidthFromPivot(newSize.x);
        rt->SetHeightFromPivot(newSize.y);
    }
}

void UIAspectRatioFitter::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("AspectRatio"))
    { SetAspectRatio(xmlInfo.Get<float>("AspectRatio")); }

    if (xmlInfo.Contains("AspectRatioMode"))
    { SetAspectRatioMode(xmlInfo.Get<AspectRatioMode>("AspectRatioMode")); }
}

void UIAspectRatioFitter::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("AspectRatio", GetAspectRatio());
    xmlInfo->Set("AspectRatioMode", GetAspectRatioMode());
}

void UIAspectRatioFitter::OnTransformChanged()
{
    ILayoutSelfController::OnTransformChanged();
    Invalidate();
}

void UIAspectRatioFitter::OnParentTransformChanged()
{
    ILayoutSelfController::OnParentTransformChanged();
    Invalidate();
}

void UIAspectRatioFitter::OnInvalidated()
{
    IInvalidatable<ILayoutController>::OnInvalidated();
}
