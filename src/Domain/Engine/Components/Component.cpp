#include "Bang/Component.h"

#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/ComponentWidget.h"

Component::Component()
{
}

Component::~Component()
{
}

void Component::OnDrawGizmos(bool depthed, bool overlay)
{
}

void Component::CloneInto(ICloneable *clone) const
{
    Object::CloneInto(clone);
    Component *c = Object::SCast<Component>(clone);
    c->SetEnabled( IsEnabled(false) );
    c->SetClosedInInspector( IsClosedInInspector() );
}

String Component::ToString() const
{
    std::ostringstream msg;
    msg << "";
    return GetClassName() + "(" + String::ToString((void*)this) + ")";
}

void Component::SetGameObject(GameObject *gameObject)
{
    m_gameObject = gameObject;
    m_gameObjectTransform = m_gameObject->transform;
    OnAddedToGameObject();
}

void Component::SetEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool Component::IsEnabled(bool recursive) const
{
    return recursive ? m_enabled && gameObject && gameObject->IsEnabled() :
                       m_enabled;
}

void Component::SetClosedInInspector(bool closed)
{
    m_closedInInspector = closed;
}

bool Component::IsClosedInInspector() const
{
    return m_closedInInspector;
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

void Component::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);

    SetEnabled(xmlInfo.GetBool("enabled"));
    SetClosedInInspector(xmlInfo.GetBool("closedInInspector"));
}

void Component::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);

    xmlInfo->SetTagName( GetClassName() );
    xmlInfo->SetString("id", GetInstanceId(),
                       {XMLProperty::Hidden, XMLProperty::Readonly});
    xmlInfo->SetBool("enabled", m_enabled,
                     {XMLProperty::Hidden, XMLProperty::Readonly});
    xmlInfo->SetBool("closedInInspector", IsClosedInInspector(),
    {XMLProperty::Hidden, XMLProperty::Readonly});
}
