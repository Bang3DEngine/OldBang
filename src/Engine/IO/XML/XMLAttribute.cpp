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

XMLAttribute::XMLAttribute(const String &name, const String &value)
{
    Set(name, value);
}


void XMLAttribute::Set(const String &name, const String &value)
{
    SetName(name);
    SetValue(value);
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
    str += "\"";
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

    int valueEnd = str.IndexOfOneOf("\"", valueBegin+1);
    if (valueEnd == -1) { return attribute; }

    String name = str.SubString(nameBegin, nameEnd-1);
    String value = str.SubString(valueBegin, valueEnd-1);
    attribute.SetName(name);
    attribute.SetValue(value);

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

