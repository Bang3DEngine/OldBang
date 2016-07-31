#include "XMLAttribute.h"

const std::vector< std::string >  XMLAttribute::TypeNames =
    {"Bool", "Int", "String", "Float",
     "Vector2", "Vector3", "Vector4",
     "Quaternion", "Rect", "File", "Enum"};

XMLAttribute::Type XMLAttribute::GetTypeFromString(const std::string &typeString)
{
    for (int i = 0; i < TypeNames.size(); ++i)
    {
        if (typeString == TypeNames[i])
        {
            return static_cast<XMLAttribute::Type>(i);
        }
    }
    return XMLAttribute::Type::TBool;
}

XMLAttribute::XMLAttribute()
{

}

XMLAttribute::XMLAttribute(const std::string &name,
                           const std::string &value,
                           XMLAttribute::Type type,
                           const std::vector<XMLProperty> &properties)
{
    Set(name, value, type, properties);
}

void XMLAttribute::Set(const std::string &name,
                       const std::string &value,
                       XMLAttribute::Type type,
                       const std::vector<XMLProperty> &properties)
{
    SetName(name);
    SetValue(value);
    SetType(type);
    SetProperties(properties);
}

void XMLAttribute::SetName(const std::string &name)
{
    m_name = name;
}

void XMLAttribute::SetValue(const std::string &value)
{
    m_value = value;
}

void XMLAttribute::SetProperty(const XMLProperty &property)
{
    SetProperty(property.GetName(), property.GetValue());
}

void XMLAttribute::SetProperty(const std::string &propertyName, const std::string &propertyValue)
{
    for (const XMLProperty& prop : m_properties) //Dont add repeated
    {
        if (prop.GetName() == propertyName)
        {
            return;
        }
    }

    XMLProperty prop(propertyName, propertyValue);
    m_properties.push_back(prop);
}

void XMLAttribute::SetProperties(const std::vector<XMLProperty> &properties)
{
    for (const XMLProperty &prop : properties)
    {
        SetProperty(prop);
    }
}

const std::string XMLAttribute::GetPropertyValue(const std::string &propertyName) const
{
    for (const XMLProperty& prop : m_properties)
    {
        if (prop.GetName() == propertyName)
        {
            return prop.GetValue();
        }
    }
    return "";
}

bool XMLAttribute::HasProperty(const XMLProperty &property) const
{
    for (const XMLProperty &prop : m_properties)
    {
        if (prop.GetName() == property.GetName())
        {
            return true;
        }
    }
    return false;
}

bool XMLAttribute::HasProperty(const std::string &propertyName) const
{
    for (const XMLProperty &prop : m_properties)
    {
        if (prop.GetName() == propertyName)
        {
            return true;
        }
    }
    return false;
}

void XMLAttribute::RemoveProperty(const std::string &propertyName)
{
    for (auto it = m_properties.begin(); it != m_properties.end(); ++it)
    {
        const XMLProperty &prop = *it;
        if (prop.GetName() == propertyName)
        {
            m_properties.erase(it);
            return;
        }
    }
}

void XMLAttribute::SetPointer(const void *value,
                              const std::vector<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << value;
    Set(m_name, oss.str(), XMLAttribute::Type::TString, properties);
}

void XMLAttribute::SetBool(bool value,
                           const std::vector<XMLProperty> &properties)
{
    Set(m_name, value ? "true" : "false", XMLAttribute::Type::TBool, properties);
}

void XMLAttribute::SetInt(int value,
                          const std::vector<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << value;
    Set(m_name, oss.str(), XMLAttribute::Type::TInt, properties);
}

void XMLAttribute::SetString(const std::string &value,
                             const std::vector<XMLProperty> &properties)
{
    Set(m_name, value, XMLAttribute::Type::TString, properties);
}

void XMLAttribute::SetFloat(float value,
                            const std::vector<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << value;
    Set(m_name, oss.str(), XMLAttribute::Type::TFloat, properties);
}

void XMLAttribute::SetVector2(const glm::vec2 &value,
                              const std::vector<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << "(" << value.x << ", " <<
                  value.y << ")";
    Set(m_name, oss.str(), XMLAttribute::Type::TVector2, properties);
}

void XMLAttribute::SetVector3(const Vector3 &value,
                              const std::vector<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << "(" << value.x << ", " <<
                  value.y << ", " <<
                  value.z << ")";
    Set(m_name, oss.str(), XMLAttribute::Type::TVector3, properties);
}

void XMLAttribute::SetVector4(const glm::vec4 &value,
                              const std::vector<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << "(" << value.x << ", " <<
                  value.y << ", " <<
                  value.z << ", " <<
                  value.w << ")";
    Set(m_name, oss.str(), XMLAttribute::Type::TVector4, properties);
}

void XMLAttribute::SetQuaternion(const Quaternion &value,
                                 const std::vector<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << "(" << value.w << ", " <<
                  value.x << ", " <<
                  value.y << ", " <<
                  value.z << ")";
    Set(m_name, oss.str(), XMLAttribute::Type::TQuaternion, properties);
}

void XMLAttribute::SetRect(const Rect &value,
                           const std::vector<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << "(" << value.m_minx << ", " <<
                  value.m_miny << ", " <<
                  value.m_maxx << ", " <<
                  value.m_maxy << ")";
    Set(m_name, oss.str(), XMLAttribute::Type::TRect, properties);
}

void XMLAttribute::SetFilepath(const std::string &filepath,
                               const std::string &fileExtension,
                               const std::vector<XMLProperty> &properties)
{
    // Add the file extension property automatically
    if (!fileExtension.empty())
    {
        XMLProperty extensionProp = XMLProperty::FileExtension;
        extensionProp.SetValue(fileExtension);
        std::vector<XMLProperty> modifiedProps = properties;
        modifiedProps.push_back(extensionProp);
        Set(m_name, filepath, XMLAttribute::Type::TFile, modifiedProps);
    }
    else
    {
        Set(m_name, filepath, XMLAttribute::Type::TFile, properties);
    }
}

void XMLAttribute::SetEnum(const std::vector<std::string> &enumNames,
                           const std::string &selectedEnumName,
                           const std::vector<XMLProperty> &properties)
{
    int selectedEnumIndex = -1;
    for (int i = 0; i < enumNames.size(); ++i)
    {
        const std::string& enumName = enumNames[i];
        if (enumName == selectedEnumName)
        {
            selectedEnumIndex = i;
            break;
        }
    }
    SetEnum(enumNames, selectedEnumIndex, properties);
}

void XMLAttribute::SetEnum(const std::vector<std::string> &enumNames,
                           int selectedEnumIndex, const std::vector<XMLProperty> &properties)
{
    Set(m_name, std::to_string(selectedEnumIndex), XMLAttribute::Type::TEnum, properties);
    for (int i = 0; i < enumNames.size(); ++i)
    {
        XMLProperty prop("EnumName" + std::to_string(i), enumNames[i]);
        SetProperty(prop);
    }
}

bool XMLAttribute::HasVectoredType() const
{
    return m_type == XMLAttribute::Type::TFloat   ||
           m_type == XMLAttribute::Type::TVector2 ||
           m_type == XMLAttribute::Type::TVector3 ||
           m_type == XMLAttribute::Type::TVector4 ||
           m_type == XMLAttribute::Type::TQuaternion;
}

int XMLAttribute::GetNumberOfFieldsOfType() const
{
    if (m_type == XMLAttribute::Type::TBool)            return 1;
    else if (m_type == XMLAttribute::Type::TInt)        return 1;
    else if (m_type == XMLAttribute::Type::TString)     return 1;
    else if (m_type == XMLAttribute::Type::TFloat)      return 1;
    else if (m_type == XMLAttribute::Type::TVector2)    return 2;
    else if (m_type == XMLAttribute::Type::TVector3)    return 3;
    else if (m_type == XMLAttribute::Type::TVector4)    return 4;
    else if (m_type == XMLAttribute::Type::TQuaternion) return 4;
    else if (m_type == XMLAttribute::Type::TRect)       return 4;
    else if (m_type == XMLAttribute::Type::TFile)       return 1;
    else if (m_type == XMLAttribute::Type::TEnum)       return 1;
    return -1;
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
    for (const XMLProperty &prop : GetProperties())
    {
        //std::cerr << "PROP: " << prop << std::endl;
        if (!first)
        {
            str += ",";
        }

        str += prop.GetName();
        if (prop.GetValue().length() > 0)
        {
            str += ":\"" + prop.GetValue() + "\"";
        }

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
    return StringUtils::ToFloat(m_value);
}

std::string XMLAttribute::GetFilepath() const
{
    return GetString();
}

std::string XMLAttribute::GetString() const
{
    return m_value;
}

glm::vec2 XMLAttribute::GetVector2() const
{
    float x = 0, y = 0;
    std::string insidePars = StringUtils::Split(m_value, '(')[1];
    insidePars =  StringUtils::Split(insidePars, ')')[0];
    std::vector<std::string> numbers = StringUtils::Split(insidePars, ',');
    x = StringUtils::ToFloat(numbers[0]);
    y = StringUtils::ToFloat(numbers[1]);
    return glm::vec2(x,y);
}

Vector3 XMLAttribute::GetVector3() const
{
    float x = 0, y = 0, z = 0;
    std::string insidePars = StringUtils::Split(m_value, '(')[1];
    insidePars =  StringUtils::Split(insidePars, ')')[0];
    std::vector<std::string> numbers = StringUtils::Split(insidePars, ',');
    x = StringUtils::ToFloat(numbers[0]);
    y = StringUtils::ToFloat(numbers[1]);
    z = StringUtils::ToFloat(numbers[2]);
    return Vector3(x,y,z);
}

glm::vec4 XMLAttribute::GetVector4() const
{
    float x = 0, y = 0, z = 0, w = 0;
    std::string insidePars = StringUtils::Split(m_value, '(')[1];
    insidePars =  StringUtils::Split(insidePars, ')')[0];
    std::vector<std::string> numbers = StringUtils::Split(insidePars, ',');
    x = StringUtils::ToFloat(numbers[0]);
    y = StringUtils::ToFloat(numbers[1]);
    z = StringUtils::ToFloat(numbers[2]);
    w = StringUtils::ToFloat(numbers[3]);
    return glm::vec4(x,y,z,w);
}

Quaternion XMLAttribute::GetQuaternion() const
{
    glm::vec4 v = GetVector4();
    return Quaternion(v.w, v.x, v.y, v.z);
}

Rect XMLAttribute::GetRect() const
{
    glm::vec4 v = GetVector4();
    return Rect(v.x, v.y, v.z, v.w);
}

std::string XMLAttribute::GetEnumSelectedName() const
{
    std::string propName = "EnumName" + std::to_string(GetEnumSelectedIndex());
    return GetPropertyValue(propName);
}

int XMLAttribute::GetEnumSelectedIndex() const
{
    return std::atoi(GetValue().c_str());
}

std::vector<std::string> XMLAttribute::GetEnumNames() const
{
    std::vector<std::string> enumNames;
    int i = 0;
    while (true)
    {
        std::string propName = "EnumName" + std::to_string(i);
        if (HasProperty(propName))
        {
            enumNames.push_back( GetPropertyValue(propName) );
        }
        else
        {
            break;
        }
        ++i;
    }
    return enumNames;
}

const std::vector<XMLProperty> &XMLAttribute::GetProperties() const
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
