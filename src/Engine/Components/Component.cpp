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
    p_gameObject = gameObject;
}

GameObject *Component::GetGameObject() const { return p_gameObject; }

void Component::OnStart() {}
void Component::OnPreUpdate() {}
void Component::OnUpdate() {}
void Component::OnPostUpdate() {}
void Component::OnBeforeChildrenRender(RenderPass renderPass) {}
void Component::OnRender(RenderPass renderPass) {}
void Component::OnRenderGizmos() {}
void Component::OnAfterChildrenRender(RenderPass renderPass) {}
void Component::OnDestroy() {}

bool Component::IsEnabled(bool recursive) const
{
    return Object::IsEnabled() && GetGameObject()->IsEnabled(recursive);
}

void Component::CloneInto(ICloneable *clone) const
{
    Serializable::CloneInto(clone);
    Component *c = SCAST<Component*>(clone);
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
