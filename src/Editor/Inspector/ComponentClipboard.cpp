#include "Bang/ComponentClipboard.h"

#include "Bang/Component.h"
#include "Bang/GameObject.h"

Component* ComponentClipboard::s_copiedComponent = nullptr;

bool ComponentClipboard::IsEmpty()
{
    return s_copiedComponent == nullptr;
}

bool ComponentClipboard::IsCopiedTransform()
{
    return !IsEmpty() && s_copiedComponent->GetClassName().Contains("Transform");
}

String ComponentClipboard::GetCopiedComponentName()
{
    return !IsEmpty() ? s_copiedComponent->GetClassName() : "";
}

void ComponentClipboard::CopyComponent(Component *componentToCopy)
{
    if (s_copiedComponent)
    {
        delete s_copiedComponent;
        s_copiedComponent = nullptr;
    }

    s_copiedComponent = componentToCopy->Clone();
}

void ComponentClipboard::PasteComponentInto(GameObject *go)
{
    go->AddComponent(s_copiedComponent->Clone());
}

void ComponentClipboard::PasteComponentValuesInto(Component *comp)
{
    s_copiedComponent->CloneInto(comp);
}
