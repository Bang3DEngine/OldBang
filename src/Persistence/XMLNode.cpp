#include "XMLNode.h"

#include "XMLParser.h"

#include "FileReader.h"
#include "FileWriter.h"
#include "StringUtils.h"
#include "IAttrWidgetButtonListener.h"

XMLNode::XMLNode(const String &tagName) :
    m_tagName(tagName)
{
}

XMLNode::~XMLNode()
{
    for (XMLNode *child : GetChildren())
    {
        delete child;
    }
}

void XMLNode::CloneInto(XMLNode *xmlNode) const
{
    xmlNode->m_children.Clear();

    String name = xmlNode->GetTagName();
    xmlNode->SetTagName(name);
    xmlNode->m_attributes = GetAttributes();
    for (XMLNode *child : m_children)
    {
        XMLNode *childClone = new XMLNode();
        child->CloneInto(childClone);
        xmlNode->AddChild(childClone);
    }
}

void XMLNode::AddChild(XMLNode *node)
{
    m_children.PushBack(node);
}

void XMLNode::UpdateAttributeValue(const String &attributeName,
                                   const String &newAttributeValue)
{
    XMLAttribute *attr = GetAttribute(attributeName);
    if (attr)
    {
        attr->SetValue(newAttributeValue);
    }
}

void XMLNode::SetAttribute(const XMLAttribute &attribute)
{
    RemoveAttribute(attribute.GetName()); // Just in case
    m_attributes[attribute.GetName()] = attribute;
}

void XMLNode::SetGenericAttribute(const String &attributeName,
                                  const String &attributeValue,
                                  XMLAttribute::Type type,
                                  const Array<XMLProperty>& properties)
{
    XMLAttribute attr(attributeName, attributeValue, type, properties);
    SetAttribute(attr);
}

void XMLNode::SetPointer(const String &attributeName, const void *value,
                         const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetPointer(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetBool(const String &attributeName, bool value,
                      const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetBool(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetInt(const String &attributeName, int value,
                     const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetInt(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetFloat(const String &attributeName, float value,
                       const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetFloat(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetVector2(const String &attributeName, const Vector2 &value,
                         const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetVector2(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetVector3(const String &attributeName, const Vector3 &value,
                         const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetVector3(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetVector4(const String &attributeName, const Vector4 &value,
                         const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetVector4(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetColor(const String &attributeName, const Color &value,
                       const Array<XMLProperty> &properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetColor(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetQuaternion(const String &attributeName, const Quaternion &value,
                            const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetQuaternion(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetRect(const String &attributeName, const Rect &value,
                      const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetRect(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetString(const String &attributeName, const String &value,
                        const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetString(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetFilepath(const String &attributeName, const String &filepath,
                          const String &fileExtension,
                          const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetFilepath(filepath, fileExtension, properties);
    SetAttribute(attr);
}

void XMLNode::SetEnum(const String &attributeName, const Array<String>& enumNames,
                      int selectedEnumIndex, const Array<XMLProperty> &properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetEnum(enumNames, selectedEnumIndex, properties);
    for (int i = 0; i < enumNames.Size(); ++i)
    {
        const String &enumName = enumNames[i];
        XMLProperty prop("EnumName" + std::to_string(i), enumName);
        attr.SetProperty(prop);
    }

    SetAttribute(attr);
}

void XMLNode::SetButton(const String &attributeName,
                        IAttrWidgetButtonListener* listener,
                        const Array<XMLProperty> &properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetButton(attributeName, listener, properties);
    SetAttribute(attr);
}

XMLAttribute* XMLNode::GetAttribute(const String &attributeName) const
{
    XMLAttribute *attr = nullptr;
    if (m_attributes.ContainsKey(attributeName))
    {
        attr = &(m_attributes[attributeName]);
    }
    return attr;
}

void XMLNode::RemoveAttribute(const String &attributeName)
{
    for (auto it = m_attributes.Begin(); it != m_attributes.End(); ++it)
    {
        XMLAttribute &attr = it->second;
        if (attr.GetName() == attributeName)
        {
            it = m_attributes.Remove(it);
            --it;
        }
    }
}

String XMLNode::GetAttributeValue(const String &attributeName) const
{
    if (m_attributes.ContainsKey(attributeName))
    {
        return m_attributes[attributeName].GetValue();
    }
    return "";
}

bool XMLNode::GetBool(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetBool() : false;
}

int XMLNode::GetInt(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetInt() : 0;
}

String XMLNode::GetString(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetString() : "";
}

String XMLNode::GetFilepath(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetFilepath() : "";
}

float XMLNode::GetFloat(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetFloat() : 0.0f;
}

Vector2 XMLNode::GetVector2(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetVector2() : Vector2::Zero;
}

Vector3 XMLNode::GetVector3(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetVector3() : Vector3::Zero;
}

Vector4 XMLNode::GetVector4(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetVector4() : Vector4::Zero;
}

Color XMLNode::GetColor(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetColor() : Color(0);
}

Quaternion XMLNode::GetQuaternion(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetQuaternion() : Quaternion();
}

Rect XMLNode::GetRect(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetRect() : Rect();
}

int XMLNode::GetEnumSelectedIndex(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetEnumSelectedIndex() : -1;
}

String XMLNode::GetEnumSelectedName(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetEnumSelectedName() : "";
}

Array<String> XMLNode::GetEnumNames(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetEnumNames() : Array<String>();
}

IAttrWidgetButtonListener *XMLNode::GetButtonListener(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetButtonListener() : nullptr;
}

const XMLNode *XMLNode::GetChild(const String &name) const
{
    for (XMLNode *node : m_children)
    {
        if (node->GetTagName() == name)
        {
            return node;
        }
    }

    return nullptr;
}

const String XMLNode::ToString(const String& indent) const
{
    String str = "";

    str += indent + "<" + m_tagName;
    for(auto itAttr : m_attributes)
    {
        XMLAttribute attr = itAttr.second;
        str += " " + attr.ToString() + "\n";
        for (int i = 0; i < m_tagName.Length() + indent.Length() + 1; ++i )
        {
            str += " ";
        }
    }
    str += ">\n";

    const String newIndent = indent + "    ";
    for(XMLNode *child : m_children)
    {
        str += child->ToString(newIndent);
    }
    str += indent + "</" + m_tagName + ">\n";
    return str;
}

void XMLNode::SetTagName(const String tagName)
{
    m_tagName = tagName;
}

const String XMLNode::ToString() const
{
    return ToString("");
}

const String &XMLNode::GetTagName() const
{
    return m_tagName;
}

const Map<String, XMLAttribute> &XMLNode::GetAttributes() const
{
    return m_attributes;
}

const List<XMLNode*>& XMLNode::GetChildren() const
{
    return m_children;
}

XMLNode *XMLNode::FromString(const String &xml)
{
    return XMLParser::FromString(xml);
}
