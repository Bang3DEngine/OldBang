#include "XMLProperty.h"

const XMLProperty XMLProperty::Hidden("Hidden");
const XMLProperty XMLProperty::Inline("Inline");
const XMLProperty XMLProperty::Readonly("Readonly");
const XMLProperty XMLProperty::FileExtension("FileExtension");

XMLProperty::XMLProperty()
{
}

XMLProperty::XMLProperty(const std::string &name, const std::string &value) :
    m_name(name), m_value(value)
{

}

void XMLProperty::SetName(const std::string &name)
{
    m_name = name;
}

void XMLProperty::SetValue(const std::string &value)
{
    m_value = value;
}

const std::string &XMLProperty::GetName() const
{
    return m_name;
}

const std::string &XMLProperty::GetValue() const
{
    return m_value;
}
