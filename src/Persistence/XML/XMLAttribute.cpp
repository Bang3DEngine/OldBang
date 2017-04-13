#include "Bang/XMLAttribute.h"

#include <iostream>
#include <functional>

#include "Bang/Debug.h"
#include "Bang/Object.h"
#include "Bang/IO.h"

#ifdef BANG_EDITOR
#include "Bang/IAttrWidgetButtonListener.h"
#endif

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

const String XMLAttribute::GetPropertyValue(const String &propertyName) const
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

bool XMLAttribute::HasProperty(const String &propertyName) const
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

void XMLAttribute::RemoveProperty(const String &propertyName)
{
    for (auto it = m_properties.Begin(); it != m_properties.End(); ++it)
    {
        const XMLProperty &prop = *it;
        if (prop.GetName() == propertyName)
        {
            m_properties.Remove(it);
            return;
        }
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

void XMLAttribute::SetVector2(const Vector2 &value,
                              const Array<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << "(" << value.x << ", " <<
                  value.y << ")";
    Set(m_name, oss.str(), XMLAttribute::Type::Vector2, properties);
}

void XMLAttribute::SetVector3(const Vector3 &value,
                              const Array<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << "(" << value.x << ", " <<
                  value.y << ", " <<
                  value.z << ")";
    Set(m_name, oss.str(), XMLAttribute::Type::Vector3, properties);
}

void XMLAttribute::SetVector4(const Vector4 &value,
                              const Array<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << "(" << value.x << ", " <<
                  value.y << ", " <<
                  value.z << ", " <<
                  value.w << ")";
    Set(m_name, oss.str(), XMLAttribute::Type::Vector4, properties);
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

void XMLAttribute::SetQuaternion(const Quaternion &value,
                                 const Array<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << "(" << value.w << ", " <<
                  value.x << ", " <<
                  value.y << ", " <<
                  value.z << ")";
    Set(m_name, oss.str(), XMLAttribute::Type::Quaternion, properties);
}

void XMLAttribute::SetRect(const Rect &value,
                           const Array<XMLProperty> &properties)
{
    std::ostringstream oss;
    oss << "(" << value.m_minx << ", " <<
                  value.m_miny << ", " <<
                  value.m_maxx << ", " <<
                  value.m_maxy << ")";
    Set(m_name, oss.str(), XMLAttribute::Type::Rect, properties);
}

void XMLAttribute::SetFilepath(const String &filepath,
                               const String &fileExtension,
                               const Array<XMLProperty> &properties)
{
    String relFilepath = filepath.Empty() ?
                             "" : IO::ToRelative(filepath);
    Set(m_name, relFilepath, XMLAttribute::Type::File, properties);

    if (!fileExtension.Empty())
    {
        XMLProperty extensionProp = XMLProperty::FileExtension;
        extensionProp.SetValue(fileExtension);
        SetProperty(extensionProp);
    }

    if (IO::IsEngineFile(filepath))
    {
        SetProperty(XMLProperty::IsEngineFile);
    }
    else
    {
        RemoveProperty(XMLProperty::IsEngineFile.GetName());
    }
}

void XMLAttribute::SetEnum(const Array<String> &enumNames,
                           const String &selectedEnumName,
                           const Array<XMLProperty> &properties)
{
    int selectedEnumIndex = -1;
    for (int i = 0; i < enumNames.Size(); ++i)
    {
        const String& enumName = enumNames[i];
        if (enumName == selectedEnumName)
        {
            selectedEnumIndex = i;
            break;
        }
    }
    SetEnum(enumNames, selectedEnumIndex, properties);
}

void XMLAttribute::SetEnum(const Array<String> &enumNames,
                           int selectedEnumIndex,
                           const Array<XMLProperty> &properties)
{
    Set(m_name, std::to_string(selectedEnumIndex),
        XMLAttribute::Type::Enum, properties);
    for (int i = 0; i < enumNames.Size(); ++i)
    {
        XMLProperty prop("EnumName" + std::to_string(i), enumNames[i]);
        SetProperty(prop);
    }
}

#ifdef BANG_EDITOR
void XMLAttribute::SetButton(const String buttonText,
                             IAttrWidgetButtonListener *listener,
                             const Array<XMLProperty> &properties)
{
    Set(m_name, buttonText, XMLAttribute::Type::Button, properties);
    std::ostringstream oss; oss << ( (void*) listener );
    XMLProperty prop("Listener", oss.str());
    SetProperty(prop);
}
#endif

bool XMLAttribute::HasVectoredType() const
{
    return m_type == XMLAttribute::Type::Int     ||
           m_type == XMLAttribute::Type::Float   ||
           m_type == XMLAttribute::Type::Vector2 ||
           m_type == XMLAttribute::Type::Vector3 ||
           m_type == XMLAttribute::Type::Vector4 ||
           m_type == XMLAttribute::Type::Rect    ||
           m_type == XMLAttribute::Type::Quaternion;
}

int XMLAttribute::GetNumberOfFieldsOfType() const
{
    if (m_type == XMLAttribute::Type::Bool)            return 1;
    else if (m_type == XMLAttribute::Type::Int)        return 1;
    else if (m_type == XMLAttribute::Type::String)     return 1;
    else if (m_type == XMLAttribute::Type::Float)      return 1;
    else if (m_type == XMLAttribute::Type::Vector2)    return 2;
    else if (m_type == XMLAttribute::Type::Vector3)    return 3;
    else if (m_type == XMLAttribute::Type::Vector4)    return 4;
    else if (m_type == XMLAttribute::Type::Color)      return 4;
    else if (m_type == XMLAttribute::Type::Quaternion) return 4;
    else if (m_type == XMLAttribute::Type::Rect)       return 4;
    else if (m_type == XMLAttribute::Type::File)       return 1;
    else if (m_type == XMLAttribute::Type::Enum)       return 1;
    else if (m_type == XMLAttribute::Type::Button)     return 1;
    return -1;
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

String XMLAttribute::GetFilepath() const
{
    return IO::ToAbsolute(GetValue(), HasProperty(XMLProperty::IsEngineFile));
}

String XMLAttribute::GetString() const
{
    return m_value;
}

Vector2 XMLAttribute::GetVector2() const
{
    float x = 0, y = 0;
    String insidePars = m_value.Split('(')[1];
    insidePars =  insidePars.Split(')')[0];
    Array<String> numbers = insidePars.Split(',');
    x = String::ToFloat(numbers[0]);
    y = String::ToFloat(numbers[1]);
    return Vector2(x,y);
}

Vector3 XMLAttribute::GetVector3() const
{
    float x = 0, y = 0, z = 0;
    String insidePars = m_value.Split('(')[1];
    insidePars =  insidePars.Split(')')[0];
    Array<String> numbers = insidePars.Split(',');
    x = String::ToFloat(numbers[0]);
    y = String::ToFloat(numbers[1]);
    z = String::ToFloat(numbers[2]);
    return Vector3(x,y,z);
}

Vector4 XMLAttribute::GetVector4() const
{
    float x = 0, y = 0, z = 0, w = 0;
    Array<String> arr = m_value.Split('(');
    String insidePars = arr[1];
    arr = insidePars.Split(')');
    insidePars =  arr[0];
    Array<String> numbers = insidePars.Split(',');
    x = String::ToFloat(numbers[0]);
    y = String::ToFloat(numbers[1]);
    z = String::ToFloat(numbers[2]);
    w = String::ToFloat(numbers[3]);
    return Vector4(x,y,z,w);
}

Color XMLAttribute::GetColor() const
{
    Color v = Color(GetVector4());
    return Color(v);
}

Quaternion XMLAttribute::GetQuaternion() const
{
    Vector4 v = GetVector4();
    return Quaternion(v.w, v.x, v.y, v.z);
}

Rect XMLAttribute::GetRect() const
{
    Vector4 v = GetVector4();
    return Rect(v.x, v.y, v.z, v.w);
}

IAttrWidgetButtonListener *XMLAttribute::GetButtonListener() const
{
    std::istringstream iss(GetPropertyValue("Listener"));
    void *lp = nullptr;
    iss >> lp;
    if (lp)
    {
        IAttrWidgetButtonListener *listener =
           Object::SCast<IAttrWidgetButtonListener>(lp);
        return listener;
    }
    return nullptr;
}

String XMLAttribute::GetEnumSelectedName() const
{
    String propName = "EnumName" + std::to_string(GetEnumSelectedIndex());
    return GetPropertyValue(propName);
}

int XMLAttribute::GetEnumSelectedIndex() const
{
    return std::atoi(GetValue().ToCString());
}

Array<String> XMLAttribute::GetEnumNames() const
{
    Array<String> enumNames;
    int i = 0;
    while (true)
    {
        String propName = "EnumName" + std::to_string(i);
        if (HasProperty(propName))
        {
            enumNames.PushBack( GetPropertyValue(propName) );
        }
        else
        {
            break;
        }
        ++i;
    }
    return enumNames;
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
