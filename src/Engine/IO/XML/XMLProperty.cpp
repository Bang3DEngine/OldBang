#include "Bang/XMLProperty.h"

#include "Bang/Array.h"

const XMLProperty XMLProperty::IsEngineFile("IsEngineFile");

XMLProperty::XMLProperty()
{
}

XMLProperty::XMLProperty(const String &name, const String &value) :
    m_name(name), m_value(value)
{
}

void XMLProperty::SetName(const String &name)
{
    m_name = name;
}

void XMLProperty::SetValue(const String &value)
{
    m_value = value;
}

const String &XMLProperty::GetName() const
{
    return m_name;
}

const String &XMLProperty::GetValue() const
{
    return m_value;
}

XMLProperty XMLProperty::FromString(const String &string)
{
    String propString = string.Trim();
    Array<String> parts = propString.Split('=');

    String name = parts[0];

    String value = "";
    if (parts.Size() > 1)
    {
        value = parts[1];
        value = value.substr(1, value.Length() - 2); // Strip the "
    }

    return XMLProperty(name, value);
}

String XMLProperty::ToString() const
{
    return GetName() + ": " + GetValue();
}

bool XMLProperty::operator==(const XMLProperty &rhs) const
{
    return m_name == rhs.m_name && m_value == rhs.m_value;
}
