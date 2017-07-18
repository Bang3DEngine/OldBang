#include "GameObjectInspectorWidgetGroup.h"

#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/InspectorWidgetFactory.h"

GameObjectInspectorWidgetGroup::GameObjectInspectorWidgetGroup(GameObject *go)
{
    p_gameObject = go;
}

GameObjectInspectorWidgetGroup::~GameObjectInspectorWidgetGroup()
{

}

String GameObjectInspectorWidgetGroup::GetTitle() const
{
    return p_gameObject->GetName();
}

void GameObjectInspectorWidgetGroup::Init()
{
    InspectorWidgetGroup::Init();
    for (Component *comp : p_gameObject->GetComponents())
    {
        InspectorWidget *compIW =
                InspectorWidgetFactory::CreateComponentWidget(comp);
        InsertInspectorWidget(compIW);
    }
    p_components = p_gameObject->GetComponents();
}

#include "Bang/Debug.h"
void GameObjectInspectorWidgetGroup::OnUpdate()
{
    InspectorWidgetGroup::OnUpdate();

    // Look for diff in the GameObject components, and reorder
    // inspector widgets as needed
    int compIndex = 0;
    auto itComp = p_components.Begin();
    auto itInspWidgets = GetInspectorWidgets().Begin();
    for (; itComp != p_components.End(); )
    {
        bool increment = true;

        Component *comp = *itComp;
        InspectorWidget *inspWidget = *itInspWidgets;
        int newIndex = p_gameObject->GetComponents().IndexOf(comp);
        if (newIndex > 0)
        {
            if (compIndex != newIndex)
            {
                Debug_Log("Move from " << compIndex << " to " << newIndex);
                Debug_Log(GetInspectorWidgets());
                MoveInspectorWidget(inspWidget, newIndex);

                auto newCompIt = p_components.Begin();
                std::advance(newCompIt, newIndex);
                p_components.InsertAfter(newCompIt, comp);
                p_components.Remove(itComp);

                itComp = p_components.Begin();
                itInspWidgets = GetInspectorWidgets().Begin();
                compIndex = 0;
                increment = false;

                Debug_Log(GetInspectorWidgets());
            }
        }

        if (increment)
        {
            ++itInspWidgets;
            ++compIndex;
            ++itComp;
        }
    }
    p_components = p_gameObject->GetComponents();
}

bool GameObjectInspectorWidgetGroup::NeedsEnableCheckBox() const
{
    return true;
}

void GameObjectInspectorWidgetGroup::OnEnableCheckBoxChanged(bool enabled)
{
    p_gameObject->SetEnabled(enabled);
    OnInspectorWidgetChanged(nullptr);
}

bool GameObjectInspectorWidgetGroup::IsEnabled() const
{
    return p_gameObject->IsEnabled();
}

List<InspectorWidget*>::Iterator
        GameObjectInspectorWidgetGroup::RemoveInspectorWidget(
                                        InspectorWidget *inspWidget)
{
    List<InspectorWidget*>::Iterator res =
             InspectorWidgetGroup::RemoveInspectorWidget(inspWidget);
    p_components = p_gameObject->GetComponents();
    return res;
}
