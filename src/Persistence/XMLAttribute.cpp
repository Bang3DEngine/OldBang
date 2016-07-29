#include "XMLAttribute.h"

const std::string XMLAttribute::PropertyHidden = "Hidden";
const std::string XMLAttribute::PropertyReadonly = "Readonly";

const std::vector< std::string >  XMLAttribute::TypeNames =
    {"Bool", "Int", "String", "Float",
     "Vector2", "Vector3", "Vector4",
     "Quaternion", "Rect", "File"};

XMLAttribute::Type XMLAttribute::GetTypeFromString(const std::string &typeString)
{
    for (int i = 0; i < TypeNames.size(); ++i)
    {
        if (typeString == TypeNames[i])
        {
            return static_cast<XMLAttribute::Type>(i);
        }
    }
    return XMLAttribute::Type::Bool;
}

XMLAttribute::XMLAttribute()
{

}

XMLAttribute::XMLAttribute(const std::string &name,
                           const std::string &value,
                           XMLAttribute::Type type,
                           const std::vector<std::string> &properties) :
    m_name(name), m_type(type),  m_value(value)
{
    for (const std::string &prop : properties)
    {
        SetProperty(prop);
    }
}

void XMLAttribute::SetName(const std::string &name)
{
    m_name = name;
}

void XMLAttribute::SetValue(const std::string &value)
{
    m_value = value;
}

void XMLAttribute::SetProperty(const std::string &property)
{
    for (const std::string& prop : m_properties)
    {
        if (prop == property)
        {
            return;
        }
    }

    m_properties.push_back(property);
}

bool XMLAttribute::HasProperty(const std::string &property) const
{
    for (const std::string &prop : m_properties)
    {
        if (prop == property)
        {
            return true;
        }
    }
    return false;
}

void XMLAttribute::RemoveProperty(const std::string &property)
{
    for (auto it = m_properties.begin(); it != m_properties.end(); ++it)
    {
        if (*it == property)
        {
            m_properties.erase(it);
            return;
        }
    }
}

void XMLAttribute::SetType(const XMLAttribute::Type &type)
{
    m_type = type;
}

const std::string XMLAttribute::ToString() const
{
    std::string str = "";
    str += GetName();
    str += ":";
    str += GetTypeName();
    str += "=\"";
    str += GetValue();
    str += "\"{";
    bool first = true;
    for (const std::string &prop : GetProperties())
    {
        //std::cerr << "PROP: " << prop << std::endl;
        if (!first)
        {
            str += ",";
        }
        str += prop.c_str();
        first = false;
    }
    str += "}";
    //std::cerr << "STR: " << str << std::endl;
    return str;
}

const std::string& XMLAttribute::GetName() const
{
    return m_name;
}

const std::string& XMLAttribute::GetValue() const
{
    return m_value;
}

const std::vector<std::string> &XMLAttribute::GetProperties() const
{
    return m_properties;
}

const XMLAttribute::Type& XMLAttribute::GetType() const
{
    return m_type;
}

const std::string& XMLAttribute::GetTypeName() const
{
    return XMLAttribute::TypeNames[m_type];
}
