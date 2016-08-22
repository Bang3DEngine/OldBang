#include "Component.h"

#include "Transform.h"
#include "GameObject.h"
#include "WindowEventManager.h"

Component::Component()
{
}

Component::~Component()
{
}

#ifdef BANG_EDITOR
void Component::OnDrawGizmos()
{
    Gizmos::Reset();
}

void Component::OnDrawGizmosNoDepth()
{
    Gizmos::Reset();
}
#endif

void Component::CloneInto(ICloneable *clone) const
{
    Component *c = static_cast<Component*>(clone);
    c->m_enabled = m_enabled;
}

const String Component::ToString() const
{
    std::ostringstream msg;
    msg << "Component";
    return msg.str();
}

String Component::GetName() const
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
