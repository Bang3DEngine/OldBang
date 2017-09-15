#include "Bang/Component.h"

#include "Bang/String.h"
#include "Bang/XMLNode.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"

USING_NAMESPACE_BANG

Component::Component()
{
}

Component::~Component()
{
}

void Component::SetGameObject(GameObject *gameObject)
{
    m_gameObject = gameObject;
}

GameObject *Component::GetGameObject() const { return m_gameObject; }

void Component::OnStart() {}
void Component::OnUpdate() {}
void Component::OnBeforeChildrenRender(RenderPass renderPass) {}
void Component::OnRender(RenderPass renderPass) {}
void Component::OnRenderGizmos() {}
void Component::OnAfterChildrenRender(RenderPass renderPass) {}
void Component::OnParentLayoutChanged() {}
void Component::OnChildLayoutChanged() {}
void Component::OnRecalculateLayout() {}
void Component::OnDestroy() {}

bool Component::IsEnabled(bool recursive) const
{
    return Object::IsEnabled() && gameObject->IsEnabled(recursive);
}

void Component::CloneInto(ICloneable *clone) const
{
    Serializable::CloneInto(clone);
    Component *c = SCAST<Component*>(clone);
    c->SetEnabled( IsEnabled() );
}

String Component::ToString() const
{
    ::std::ostringstream msg;
    msg << "";
    return GetClassName() + "(" + String::ToString((void*)this) + ")";
}

String Component::GetInstanceId() const
{
    String instanceId = "Component";
    if (gameObject)
    {
        instanceId.Prepend(gameObject->GetInstanceId() + "_");
        Component *ncThis = const_cast<Component*>(this);
        int indInGameObject = gameObject->GetComponents().IndexOf(ncThis);
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
