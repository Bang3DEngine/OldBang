#include "EditorUIGameObject.h"

EditorUIGameObject::EditorUIGameObject()
{

}

EditorUIGameObject::~EditorUIGameObject()
{

}

void EditorUIGameObject::CloneInto(ICloneable *clone) const
{
    UIGameObject::CloneInto(clone);
    EditorGameObject::CloneInto(clone);
    EditorUIGameObject *go = dynamic_cast<EditorUIGameObject*>(clone);
    go->m_renderInSelectionFramebuffer = m_renderInSelectionFramebuffer;
}

ICloneable *EditorUIGameObject::Clone() const
{
    EditorUIGameObject *clone = new EditorUIGameObject();
    CloneInto(clone);
    return clone;
}

void EditorUIGameObject::FillXMLInfo(XMLNode *xmlInfo) const
{
    EditorGameObject::FillXMLInfo(xmlInfo);
}

