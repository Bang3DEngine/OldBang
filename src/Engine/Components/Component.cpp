#include "Bang/Component.h"

#include "Bang/String.h"
#include "Bang/XMLNode.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"

Component::Component()
{
}

Component::~Component()
{
}

void Component::CloneInto(ICloneable *clone) const
{
    Object::CloneInto(clone);
    Component *c = SCAST<Component*>(clone);
    c->SetEnabled( IsEnabled(false) );
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

    SetEnabled(xmlInfo.Get<bool>("enabled"));
}

void Component::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);

    xmlInfo->SetTagName( GetClassName() );
    xmlInfo->Set("id", GetInstanceId());
    xmlInfo->Set("enabled", m_enabled);
}
