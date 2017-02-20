#include "EditorGameObject.h"

#include "Component.h"

EditorGameObject::EditorGameObject(const String &name) : GameObject(name)
{
    AddHideFlag(HideFlags::HideAndDontSave);
}

EditorGameObject::~EditorGameObject()
{
}
