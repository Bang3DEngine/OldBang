#include "Bang/Canvas.h"

#include "Bang/XMLNode.h"

Canvas::Canvas()
{
}

Canvas::~Canvas()
{

}

void Canvas::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Canvas *c = SCAST<Canvas*>(clone);
}

void Canvas::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);
}

void Canvas::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);
}
