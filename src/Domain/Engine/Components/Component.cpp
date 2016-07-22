#include "Component.h"
#include "GameObject.h"

Component::Component()
{
}

Component::~Component()
{
#ifdef BANG_EDITOR
    for (int i = 0; i < m_inspectorComponentInfo.GetSlotInfos().size(); ++i)
        delete m_inspectorComponentInfo.GetSlotInfo(i);
#endif
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

std::string Component::GetTag() const
{
    return GetName();
}

void Component::WriteInternal(std::ostream &f) const
{
    FileWriter::Write(((void*)this), f);
    FileWriter::WriteBool(m_enabled, f);
}

void Component::ReadInternal(std::istream &f)
{
    FileReader::RegisterNextPointerId(f, this);
    m_enabled = FileReader::ReadBool(f);
}
