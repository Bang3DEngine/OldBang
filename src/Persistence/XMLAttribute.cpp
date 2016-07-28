#include "XMLAttribute.h"


const std::string XMLAttribute::TypeNames[11] =
    {"Bool", "Int", "String", "Float",
     "Vector2", "Vector3", "Vector4",
     "Quaternion", "Rect", "Enum", "File"};

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
