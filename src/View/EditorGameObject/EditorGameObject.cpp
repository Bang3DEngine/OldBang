#include "EditorGameObject.h"

EditorGameObject::EditorGameObject() : GameObject()
{
}

EditorGameObject::EditorGameObject(const std::string &name) : GameObject(name)
{
}

void EditorGameObject::Write(std::ostream &f) const
{
    //Won't be written (can't be saved)
}

bool EditorGameObject::IsEditorGameObject() const
{
    return true;
}
