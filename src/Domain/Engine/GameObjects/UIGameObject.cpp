#include "UIGameObject.h"

#include "Debug.h"
#include "XMLNode.h"
#include "ICloneable.h"
#include "EditorState.h"
#include "RectTransform.h"

UIGameObject::UIGameObject(const String &name)
    : GameObject(name)
{
    RemoveComponent<Transform>();
    AddComponent<RectTransform>();
}

void UIGameObject::CloneInto(ICloneable *clone) const
{
    UIGameObject *go = dynamic_cast<UIGameObject*>(clone);
    GameObject::CloneInto(go);
}

ICloneable *UIGameObject::Clone() const
{
    UIGameObject *clone = new UIGameObject();
    CloneInto(clone);
    return clone;
}

void UIGameObject::FillXMLInfo(XMLNode *xmlInfo) const
{
    GameObject::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("UIGameObject");
}

#ifdef BANG_EDITOR
void UIGameObject::OnHierarchyGameObjectsSelected(
        List<GameObject *> &selectedGameObjects)
{
    GameObject::OnHierarchyGameObjectsSelected(selectedGameObjects);
    if (IsEditorGameObject() || IsScene()) return;

    if ( IsSelected() )
    {
        EditorState::SetTransformMode(
                    EditorState::TransformMode::RectTransform);
    }
}
#endif
