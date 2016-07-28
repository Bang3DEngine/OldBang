#include "XMLAttribute.h"


const std::vector< std::string >  XMLAttribute::TypeNames =
    {"Bool", "Int", "String", "Float",
     "Vector2", "Vector3", "Vector4",
     "Quaternion", "Rect", "Enum", "File"};

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
                           XMLAttribute::Type type) :
    m_name(name), m_value(value), m_type(type)
{

}

void XMLAttribute::SetName(const std::string &name)
{
    m_name = name;
}

void XMLAttribute::SetValue(const std::string &value)
{
    m_value = value;
}

void XMLAttribute::SetType(const XMLAttribute::Type &type)
{
    m_type = type;
}

std::string XMLAttribute::ToString() const
{
    std::string str = "";
    str += GetName();
    str += ":";
    str += GetTypeName();
    str += "=\"";
    str += GetValue();
    str += "\"";
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

const XMLAttribute::Type& XMLAttribute::GetType() const
{
    return m_type;
}

const std::string& XMLAttribute::GetTypeName() const
{
    return XMLAttribute::TypeNames[m_type];
}
