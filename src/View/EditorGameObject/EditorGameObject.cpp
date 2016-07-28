#include "EditorGameObject.h"

#include "Component.h"

EditorGameObject::EditorGameObject() : GameObject()
{
}

EditorGameObject::EditorGameObject(const std::string &name) : GameObject(name)
{
}

void EditorGameObject::CloneInto(ICloneable *clone) const
{
    GameObject::CloneInto(clone);
    EditorGameObject *go = static_cast<EditorGameObject*>(clone);
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

void EditorGameObject::FillXMLInfo(XMLNode *xmlInfo) const
{
    xmlInfo = nullptr;
    // Won't be written (can't be saved)
    // So we need this method to be "empty"
}
