#include "Canvas.h"

Canvas::Canvas()
{
}

Canvas::~Canvas()
{

}

const String Canvas::ToString() const
{
    return "Canvas";
}

void Canvas::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Canvas *c = static_cast<Canvas*>(clone);
}

ICloneable *Canvas::Clone() const
{
    Canvas *cv = new Canvas();
    CloneInto(cv);
    return cv;
}

#ifdef BANG_EDITOR
void Canvas::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void Canvas::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}
#endif

void Canvas::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);
}

void Canvas::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
}
