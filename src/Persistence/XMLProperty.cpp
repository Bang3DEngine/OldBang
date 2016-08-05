#include "XMLProperty.h"

const XMLProperty XMLProperty::Hidden("Hidden");
const XMLProperty XMLProperty::Inline("Inline");
const XMLProperty XMLProperty::BigText("BigText");
const XMLProperty XMLProperty::Readonly("Readonly");
const XMLProperty XMLProperty::Disabled("Disabled");
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

XMLProperty XMLProperty::FromString(const std::string &string)
{
    std::string propString = string;

    StringUtils::Trim(&propString);
    std::vector<std::string> parts = StringUtils::Split(propString, '=');

    std::string name = parts[0];

    std::string value = "";
    if (parts.size() > 1)
    {
        value = parts[1];
        value = value.substr(1, value.length()-2); // Strip the "
    }

    return XMLProperty(name, value);
}
