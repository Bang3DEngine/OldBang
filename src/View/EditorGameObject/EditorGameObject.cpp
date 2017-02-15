#include "EditorGameObject.h"

#include "Component.h"

EditorGameObject::EditorGameObject() : GameObject()
{
}

EditorGameObject::EditorGameObject(const String &name) : GameObject(name)
{
}

EditorGameObject::~EditorGameObject()
{
}

void EditorGameObject::CloneInto(ICloneable *clone) const
{
    GameObject::CloneInto(clone);
    EditorGameObject *go = dynamic_cast<EditorGameObject*>(clone);
    go->m_renderInSelectionFramebuffer = m_renderInSelectionFramebuffer;
}

ICloneable* EditorGameObject::Clone() const
{
    EditorGameObject *clone = new EditorGameObject();
    CloneInto(clone);
    return clone;
}

bool EditorGameObject::IsEditorGameObject() const
{
    return true;
}

bool EditorGameObject::IsRenderInSelectionFramebuffer() const
{
    return m_renderInSelectionFramebuffer;
}

void EditorGameObject::FillXMLInfo(XMLNode *xmlInfo) const
{
    xmlInfo = nullptr;
    // Won't be written (can't be saved)
    // So we need this method to be "empty"
}
