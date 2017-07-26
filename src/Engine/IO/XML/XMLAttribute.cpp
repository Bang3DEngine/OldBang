#include "Bang/XMLAttribute.h"

#include <iostream>
#include <functional>

#include "Bang/Paths.h"
#include "Bang/Object.h"

XMLAttribute::XMLAttribute()
{
}

XMLAttribute::XMLAttribute(const String &name,
                           const String &value,
                           XMLAttribute::Type type,
                           const Array<XMLProperty> &properties)
{
    Set(name, value, type, properties);
}

void XMLAttribute::Set(const String &name,
                       const String &value,
                       XMLAttribute::Type type,
                       const Array<XMLProperty> &properties)
{
    SetName(name);
    SetValue(value);
    SetType(type);
    SetProperties(properties);
}

void XMLAttribute::SetName(const String &name)
{
    m_name = name;
}

void XMLAttribute::SetValue(const String &value)
{
    m_value = value;
}

void XMLAttribute::SetProperty(const XMLProperty &property)
{
    SetProperty(property.GetName(), property.GetValue());
}

void XMLAttribute::SetProperty(const String &propertyName, const String &propertyValue)
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

void XMLAttribute::SetPointer(const void *value,
                              const Array<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << value;
    Set(m_name, oss.str(), XMLAttribute::Type::String, properties);
}

void XMLAttribute::SetBool(bool value,
                           const Array<XMLProperty> &properties)
{
    Set(m_name, value ? "true" : "false", XMLAttribute::Type::Bool, properties);
}

void XMLAttribute::SetInt(int value,
                          const Array<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << value;
    Set(m_name, oss.str(), XMLAttribute::Type::Int, properties);
}

void XMLAttribute::SetString(const String &value,
                             const Array<XMLProperty> &properties)
{
    Set(m_name, value, XMLAttribute::Type::String, properties);
}

void XMLAttribute::SetFloat(float value,
                            const Array<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << value;
    Set(m_name, oss.str(), XMLAttribute::Type::Float, properties);
}

void XMLAttribute::SetColor(const Color &value,
                            const Array<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << "(" << value.r << ", " <<
                  value.g << ", " <<
                  value.b << ", " <<
                  value.a << ")";
    Set(m_name, oss.str(), XMLAttribute::Type::Color, properties);
}

void XMLAttribute::SetFilepath(const Path &filepath,
                               const Array<XMLProperty> &properties)
{
    String relFilepath = Paths::GetRelative(filepath).ToString();
    Set(m_name, relFilepath, XMLAttribute::Type::File, properties);

    if (Paths::IsEnginePath(filepath))
    {
        SetProperty(XMLProperty::IsEngineFile);
    }
    else
    {
        RemoveProperty(XMLProperty::IsEngineFile.GetName());
    }
}

void XMLAttribute::SetType(const XMLAttribute::Type &type)
{
    m_type = type;
}

String XMLAttribute::ToString() const
{
    String str = "";
    str += GetName();
    str += ":";
    str += GetTypeName();
    str += "=\"";
    str += GetValue();
    str += "\"{";
    bool first = true;
    for (const XMLProperty &prop : GetProperties())
    {
        if (!first) { str += ","; }
        str += prop.GetName();
        if (!prop.GetValue().Empty())
        {
            str += "=\"" + prop.GetValue() + "\"";
        }

        first = false;
    }
    str += "}";
    return str;

}

const String& XMLAttribute::GetName() const
{
    return m_name;
}

String XMLAttribute::GetValue() const
{
    return m_value;
}

bool XMLAttribute::GetBool() const
{
    return m_value == "true";
}

int XMLAttribute::GetInt() const
{
    std::istringstream iss(m_value);
    int v; iss >> v;
    return v;
}

float XMLAttribute::GetFloat() const
{
    return String::ToFloat(m_value);
}

Path XMLAttribute::GetFilepath() const
{
    if ( GetValue().Empty() ) { return Path(); }

    if ( HasProperty(XMLProperty::IsEngineFile) ) { return EPATH(GetValue()); }
    else { return PPATH(GetValue()); }
}

String XMLAttribute::GetString() const
{
    return m_value;
}

Color XMLAttribute::GetColor() const
{
    Color v = Color(GetVector4());
    return Color(v);
}

const Array<XMLProperty> &XMLAttribute::GetProperties() const
{
    return m_properties;
}

XMLAttribute XMLAttribute::FromString(const String &string)
{
    XMLAttribute attribute;

    String str = string.Trim();

    const String TokenSpace = " \t\n";
    int nameBegin = str.IndexOfOneNotOf(TokenSpace, 0);
    if (nameBegin == -1) { return attribute; }

    int nameEnd = str.IndexOfOneOf(TokenSpace + ":", nameBegin + 1);
    if (nameEnd == -1) { return attribute; }

    int typeBegin = str.IndexOfOneNotOf(TokenSpace, nameEnd + 1);
    if (typeBegin == -1) { return attribute; }

    int typeEnd = str.IndexOfOneOf(TokenSpace + "=", typeBegin + 1);
    if (typeEnd == -1) { return attribute; }

    int valueBegin = str.IndexOfOneOf("\"", typeEnd + 1) + 1;
    if (valueBegin == -1) { return attribute; }

    int valueEnd = str.IndexOfOneOf("\"", valueBegin);
    if (valueEnd == -1) { return attribute; }

    int propertiesBegin = str.IndexOfOneOf("{", valueEnd) + 1;
    if (propertiesBegin == -1) { return attribute; }

    int propertiesEnd = str.IndexOfOneOf("}", propertiesBegin);
    if (propertiesEnd == -1) { return attribute; }

    String name = str.SubString(nameBegin, nameEnd-1);
    String typeString = str.SubString(typeBegin, typeEnd-1);
    String value = str.SubString(valueBegin, valueEnd-1);
    String propertiesString = str.SubString(propertiesBegin, propertiesEnd-1);
    Array<String> properties = propertiesString.Split(',');

    attribute.SetName(name);
    attribute.SetType(XMLAttribute::Type_FromString(typeString));
    attribute.SetValue(value);

    for (String propString : properties)
    {
        XMLProperty prop = XMLProperty::FromString(propString);
        attribute.SetProperty(prop);
    }

    return attribute;
}

const XMLAttribute::Type& XMLAttribute::GetType() const
{
    return m_type;
}


String XMLAttribute::GetTypeName() const
{
    return Type_ToString(m_type);
}
