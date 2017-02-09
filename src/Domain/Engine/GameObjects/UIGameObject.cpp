#include "UIGameObject.h"

#include "Debug.h"
#include "XMLNode.h"
#include "RectTransform.h"

UIGameObject::UIGameObject(const String &name)
    : GameObject(name)
{
    RemoveComponent<Transform>();
    AddComponent<RectTransform>();
}

void UIGameObject::FillXMLInfo(XMLNode *xmlInfo) const
{
    GameObject::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("UIGameObject");
}
