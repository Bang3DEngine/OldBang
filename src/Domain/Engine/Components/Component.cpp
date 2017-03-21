#include "Component.h"

#include "Debug.h"
#include "XMLNode.h"
#include "Transform.h"
#include "GameObject.h"

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
    c->m_enabled = m_enabled;
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

bool Component::IsEnabled() const
{
    return gameObject && gameObject->IsEnabled() && m_enabled;
}

void Component::SetClosedInInspector(bool closed)
{
    m_closedInInspector = closed;
}

bool Component::IsClosedInInspector() const
{
    return m_closedInInspector;
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
    xmlInfo->SetPointer("id", this,
                        {XMLProperty::Hidden, XMLProperty::Readonly});
    xmlInfo->SetBool("enabled", m_enabled,
                     {XMLProperty::Hidden, XMLProperty::Readonly});
    xmlInfo->SetBool("closedInInspector", IsClosedInInspector(),
                     {XMLProperty::Hidden, XMLProperty::Readonly});
}
