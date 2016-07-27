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

void Component::ReadXMLNode(const XMLNode *xmlNode)
{
    // FileReader::RegisterNextPointerId(f, this);
    m_enabled = xmlNode->GetBool("enabled");
}

void Component::GetXMLNode(XMLNode *xmlNode) const
{
    xmlNode->SetTagName("Component");
    xmlNode->AddAttribute("id", this);
    xmlNode->AddAttribute("enabled", m_enabled);
}
