#include "Canvas.h"

#include "XMLNode.h"

Canvas::Canvas()
{
}

Canvas::~Canvas()
{

}

String Canvas::GetName() const
{
    return "Canvas";
}

void Canvas::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Canvas *c = Object::SCast<Canvas>(clone);
}

ICloneable *Canvas::Clone() const
{
    Canvas *cv = new Canvas();
    CloneInto(cv);
    return cv;
}

void Canvas::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);
}

void Canvas::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName( GetName() );
}
