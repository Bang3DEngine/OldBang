#include "EditorGameObject.h"

EditorGameObject::EditorGameObject() : GameObject()
{
}

EditorGameObject::EditorGameObject(const std::string &name) : GameObject(name)
{
}

void EditorGameObject::Write(std::ostream &f) const
{
    // Won't be written (can't be saved)
    // So we need this method to be empty
}

bool EditorGameObject::IsEditorGameObject() const
{
    return true;
}

GameObject *EditorGameObject::GetAttachedGameObject() const
{
    GameObject *parent = GetParent();
    if(parent )
    {
        while(parent->IsEditorGameObject())
        {
            if(parent->GetParent() == nullptr) break;
            parent = parent->GetParent();
        }
    }

    if(!parent->IsEditorGameObject()) return parent;
    return nullptr;
}
