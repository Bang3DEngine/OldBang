#include "ComponentClipboard.h"

#include "Component.h"
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

String ComponentClipboard::GetCopiedComponentName()
{
    if (!IsEmpty())
    {
        return s_copiedComponent->GetName();
    }
    return "";
}

void ComponentClipboard::CopyComponent(Component *componentToCopy)
{
    if (s_copiedComponent)
    {
        delete s_copiedComponent;
        s_copiedComponent = nullptr;
    }

    s_copiedComponent = Object::SCast<Component>(componentToCopy->Clone());
}

void ComponentClipboard::PasteComponentInto(GameObject *go)
{
    go->AddComponent( Object::SCast<Component>(s_copiedComponent->Clone()) );
}

void ComponentClipboard::PasteComponentValuesInto(Component *comp)
{
    s_copiedComponent->CloneInto(comp);
}
