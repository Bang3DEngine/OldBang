#include "Bang/UIComponent.h"

#include "Bang/UIGameObject.h"

UIComponent::UIComponent()
{

}

UIComponent::~UIComponent()
{

}

UIGameObject *UIComponent::GetGameObject() const
{
    GameObject *go = Component::GetGameObject();
    return go ? DCAST<UIGameObject*>(go) : nullptr;
}
