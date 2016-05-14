#include "EditorGameObject.h"

EditorGameObject::EditorGameObject() : GameObject()
{
}

EditorGameObject::EditorGameObject(const std::string &name) : GameObject(name)
{
}

bool EditorGameObject::IsEditorGameObject() const
{
    return true;
}
