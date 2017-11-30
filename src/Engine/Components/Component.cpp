#include "Bang/Component.h"

#include "Bang/String.h"
#include "Bang/XMLNode.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"

USING_NAMESPACE_BANG

Component::Component()
{
    ASSERT(ObjectManager::AssertCreatedFromObjectManager);
}

Component::~Component()
{
    ASSERT(ObjectManager::AssertDestroyedFromObjectManager);
    SetGameObject(nullptr);
}

void Component::Destroy(Component *component)
{
    ObjectManager::Destroy(component);
}

void Component::SetGameObject(GameObject *gameObject)
{
    if (GetGameObject() != gameObject)
    {
        if (GetGameObject())
        {
            GetGameObject()->_RemoveComponent(this);
        }

        if (!CanBeRepeatedInGameObject())
        {
            ASSERT(!gameObject->HasComponent(GetClassName()));
        }

        p_gameObject = gameObject;
    }
}

GameObject *Component::GetGameObject() const { return p_gameObject; }

void Component::OnStart() {}
void Component::OnPreUpdate() {}
void Component::OnBeforeChildrenUpdate() {}
void Component::OnUpdate() {}
void Component::OnAfterChildrenUpdate() {}
void Component::OnPostUpdate() {}
void Component::OnBeforeChildrenRender(RenderPass) {}
void Component::OnRender(RenderPass) {}
void Component::OnRenderGizmos() {}
void Component::OnAfterChildrenRender(RenderPass) {}
void Component::OnDestroy() {}

bool Component::CanBeRepeatedInGameObject() const
{
    return true;

}

bool Component::IsEnabled(bool recursive) const
{
    return recursive ?
             (Object::IsEnabled() && GetGameObject()->IsEnabled(true)) :
              Object::IsEnabled();
}

void Component::CloneInto(ICloneable *clone) const
{
    Object::CloneInto(clone);
    Component *c = Cast<Component*>(clone);
    c->SetEnabled( IsEnabled() );
}

String Component::ToString() const
{
    std::ostringstream msg;
    msg << "";
    return GetClassName() + "(" + String::ToString((void*)this) + ")";
}

String Component::GetInstanceId() const
{
    String instanceId = GetClassName();
    if (GetGameObject())
    {
        instanceId.Prepend(GetGameObject()->GetInstanceId() + "_");
        Component *ncThis = const_cast<Component*>(this);
        int indInGameObject = GetGameObject()->GetComponents().IndexOf(ncThis);
        instanceId.Append( String::ToString(indInGameObject) );
    }
    return instanceId;
}

void Component::ImportXML(const XMLNode &xmlInfo)
{
    Serializable::ImportXML(xmlInfo);
    if (xmlInfo.Contains("Enabled"))
    { SetEnabled(xmlInfo.Get<bool>("Enabled", true)); }
}

void Component::ExportXML(XMLNode *xmlInfo) const
{
    Serializable::ExportXML(xmlInfo);

    xmlInfo->SetTagName( GetClassName() );
    xmlInfo->Set("Enabled", IsEnabled());
}
