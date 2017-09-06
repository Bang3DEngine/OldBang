#include "Bang/XMLAttribute.h"

#include <iostream>
#include <functional>

#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"

USING_NAMESPACE_BANG

XMLAttribute::XMLAttribute()
{
}

XMLAttribute::XMLAttribute(const String &name,
                           const String &value,
                           const Array<XMLProperty> &properties)
{
    Set(name, value, properties);
}


void XMLAttribute::Set(const String &name, const String &value,
                        const Array<XMLProperty> &properties)
{
    SetName(name);
    SetValue(value);
    SetProperties(properties);
}

const String &XMLAttribute::GetName() const
{
    return m_name;
}

const String& XMLAttribute::GetStringValue() const
{
    return m_value;
}

String XMLAttribute::ToString() const
{
    String str = "";
    str += GetName();
    str += "=\"";
    str += GetStringValue();
    str += "\"{";
    bool first = true;
    for (const XMLProperty &prop : GetProperties())
    {
        if (!first) { str += ","; }
        str += prop.GetName();
        if (!prop.GetValue().IsEmpty())
        {
            str += "=\"" + prop.GetValue() + "\"";
        }

        first = false;
    }
    str += "}";
    return str;
}

XMLAttribute XMLAttribute::FromString(const String &string)
{
    XMLAttribute attribute;

    String str = string.Trim();

    const String TokenSpace = " \t\n";
    int nameBegin = str.IndexOfOneNotOf(TokenSpace, 0);
    if (nameBegin == -1) { return attribute; }

    int nameEnd = str.IndexOfOneOf(TokenSpace + ":" + "=", nameBegin + 1);
    if (nameEnd == -1) { return attribute; }

    int valueBegin = str.IndexOfOneOf("\"", nameEnd + 1) + 1;
    if (valueBegin == -1) { return attribute; }

    int valueEnd = str.IndexOfOneOf("\"", valueBegin);
    if (valueEnd == -1) { return attribute; }

    int propertiesBegin = str.IndexOfOneOf("{", valueEnd) + 1;
    if (propertiesBegin == -1) { return attribute; }

    int propertiesEnd = str.IndexOfOneOf("}", propertiesBegin);
    if (propertiesEnd == -1) { return attribute; }

    String name = str.SubString(nameBegin, nameEnd-1);
    String value = str.SubString(valueBegin, valueEnd-1);
    attribute.SetName(name);
    attribute.SetValue(value);

    String propertiesString = str.SubString(propertiesBegin, propertiesEnd-1);
    Array<String> properties = propertiesString.Split<Array>(',');
    for (const String& propString : properties)
    {
        XMLProperty prop = XMLProperty::FromString(propString);
        attribute.SetProperty(prop);
    }

    return attribute;
}

void XMLAttribute::SetValue(const String &value)
{
    m_value = value;
}

void XMLAttribute::SetName(const String &name)
{
    m_name = name;
}

void XMLAttribute::SetProperty(const XMLProperty &property)
{
    SetProperty(property.GetName(), property.GetValue());
}

void XMLAttribute::SetProperty(const String &propertyName,
                               const String &propertyValue)
{
    for (const XMLProperty& prop : m_properties) //Dont add repeated
    {
        ENSURE (prop.GetName() != propertyName);
    }

    XMLProperty prop(propertyName, propertyValue);
    m_properties.PushBack(prop);
}

void XMLAttribute::SetProperties(const Array<XMLProperty> &properties)
{
    for (const XMLProperty &prop : properties)
    {
        SetProperty(prop);
    }
}

const String& XMLAttribute::GetPropertyValue(const String &propertyName) const
{
    for (const XMLProperty& prop : m_properties)
    {
        if (prop.GetName() == propertyName)
        {
            return prop.GetValue();
        }
    }
    static String emptyString;
    return emptyString;
}

bool XMLAttribute::HasProperty(const XMLProperty &property) const
{
    return HasProperty(property.GetName());
}

bool XMLAttribute::HasProperty(const String &propertyName) const
{
    for (const XMLProperty &prop : m_properties)
    {
        if (prop.GetName() == propertyName) { return true; }
    }
    return false;
}

void XMLAttribute::RemoveProperty(const String &propertyName)
{
    for (auto it = m_properties.Begin(); it != m_properties.End();)
    {
        const XMLProperty &prop = *it;
        if (prop.GetName() == propertyName)
        {
            it = m_properties.Remove(it);
        }
        else { ++it; }
    }
}

const Array<XMLProperty> &XMLAttribute::GetProperties() const
{
    return m_properties;
}


