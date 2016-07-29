#include "Component.h"
#include "GameObject.h"

Component::Component()
{
}

Component::~Component()
{
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

void Component::SetEnabled(bool enabled)
{
    this->m_enabled = enabled;
}

bool Component::IsEnabled()
{
    return m_enabled;
}

void Component::ReadXMLInfo(const XMLNode *xmlInfo)
{
    // FileReader::RegisterNextPointerId(f, this);
    m_enabled = xmlInfo->GetBool("enabled");
}

void Component::FillXMLInfo(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("Component");
    xmlInfo->SetPointer("id", this,
                        {XMLProperty::Hidden,
                         XMLProperty::Readonly});
    xmlInfo->SetBool("enabled", m_enabled,
                     {XMLProperty::Hidden,
                      XMLProperty::Readonly});
}
