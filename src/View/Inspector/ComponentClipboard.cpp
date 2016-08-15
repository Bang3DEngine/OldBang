#include "ComponentClipboard.h"

#include "GameObject.h"

Component* ComponentClipboard::s_copiedComponent = nullptr;

bool ComponentClipboard::IsEmpty()
{
    return s_copiedComponent == nullptr;
}

bool ComponentClipboard::IsCopiedTransform()
{
    return !IsEmpty() && s_copiedComponent->GetName() == "Transform";
}

void ComponentClipboard::CopyComponent(Component *componentToCopy)
{
    if (s_copiedComponent)
    {
        delete s_copiedComponent;
        s_copiedComponent = nullptr;
    }

    s_copiedComponent = static_cast<Component*>(componentToCopy->Clone());
    Logger_Log(s_copiedComponent);
}

void ComponentClipboard::PasteComponentInto(GameObject *go)
{
    go->AddComponent( static_cast<Component*>(s_copiedComponent->Clone()) );
}

void ComponentClipboard::PasteComponentValuesInto(Component *comp)
{
    s_copiedComponent->CloneInto(comp);
}
