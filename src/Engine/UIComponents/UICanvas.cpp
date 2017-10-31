#include "Bang/UICanvas.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

UICanvas::UICanvas()
{
}

UICanvas::~UICanvas()
{
}

void UICanvas::OnUpdate()
{
    Component::OnUpdate();
}

void UICanvas::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
}

void UICanvas::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);
}

void UICanvas::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);
}
