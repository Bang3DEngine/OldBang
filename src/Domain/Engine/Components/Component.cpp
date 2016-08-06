#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

Component::Component()
{
}

Component::~Component()
{
}

void Component::OnDrawGizmos()
{
    Gizmos::Reset();
}

void Component::OnDrawGizmosNoDepth()
{
    Gizmos::Reset();
}

void Component::CloneInto(ICloneable *clone) const
{
    Component *c = static_cast<Component*>(clone);
    c->m_enabled = m_enabled;
}

const std::string Component::ToString() const
{
    std::ostringstream msg;
    msg << "Component";
    return msg.str();
}

std::string Component::GetName() const
{
    return "Component";
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

bool Component::IsEnabled() const
{
    return m_enabled;
}

void Component::ReadXMLInfo(const XMLNode *xmlInfo)
{
    IFileable::ReadXMLInfo(xmlInfo);

    SetEnabled(xmlInfo->GetBool("enabled"));
}

void Component::FillXMLInfo(XMLNode *xmlInfo) const
{
    IFileable::FillXMLInfo(xmlInfo);

    xmlInfo->SetTagName("Component");
    xmlInfo->SetPointer("id", this,
                        {XMLProperty::Hidden,
                         XMLProperty::Readonly});
    xmlInfo->SetBool("enabled", IsEnabled(),
                     {XMLProperty::Hidden,
                      XMLProperty::Readonly});
}
