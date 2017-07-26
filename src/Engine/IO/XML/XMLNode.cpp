#include "Bang/XMLNode.h"

#include "Bang/Paths.h"
#include "Bang/XMLParser.h"
#include "Bang/FileReader.h"

XMLNode::XMLNode(const String &tagName)
{
    SetTagName(tagName);
}

XMLNode::~XMLNode()
{
}

void XMLNode::CloneInto(XMLNode *xmlNode) const
{
    xmlNode->m_children.Clear();

    String name = xmlNode->GetTagName();
    xmlNode->SetTagName(name);
    xmlNode->m_attributes = GetAttributes();
    xmlNode->m_attributeOrder = GetAttributesOrderList();
    for (const XMLNode& child : m_children)
    {
        XMLNode childClone;
        child.CloneInto(&childClone);
        xmlNode->AddChild(childClone);
    }
}

void XMLNode::AddChild(const XMLNode &node)
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
    XMLAttribute *attr = GetAttribute(attribute.GetName());
    if (!attr)
    {
        m_attributeOrder.PushBack(attribute.GetName());
        m_attributes[attribute.GetName()] = attribute;
    }
    else
    {
        *attr = attribute;
    }
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

void XMLNode::SetColor(const String &attributeName, const Color &value,
                       const Array<XMLProperty> &properties)

{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetColor(value, properties);
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

void XMLNode::SetFilepath(const String &attributeName, const Path &filepath,
                          const Array<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetFilepath(filepath, properties);
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
    for (auto it = m_attributes.Begin(); it != m_attributes.End();)
    {
        XMLAttribute &attr = it->second;
        if (attr.GetName() == attributeName)
        {
            m_attributes.Remove(it++);
            m_attributeOrder.Remove(attr.GetName());
        }
        else { ++it; }
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

Path XMLNode::GetFilepath(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetFilepath() : Path();
}

float XMLNode::GetFloat(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetFloat() : 0.0f;
}

Color XMLNode::GetColor(const String &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetColor() : Color(0);
}

const XMLNode *XMLNode::GetChild(const String &name) const
{
    for (const XMLNode& node : m_children)
    {
        if (node.GetTagName() == name) { return &node; }
    }
    return nullptr;
}

String XMLNode::ToString(const String& indent) const
{
    String str = "";

    str += indent + "<" + m_tagName;
    for (auto itAttr : GetAttributesListInOrder())
    {
        const XMLAttribute& attr = itAttr.second;
        str += " " + attr.ToString() + "\n";
        for (int i = 0; i < m_tagName.Length() + indent.Length() + 1; ++i )
        {
            str += " ";
        }
    }
    str += ">\n";

    const String newIndent = indent + "    ";
    for (const XMLNode& child : m_children)
    {
        str += child.ToString(newIndent);
    }
    str += indent + "</" + m_tagName + ">\n";
    return str;
}

void XMLNode::SetTagName(const String tagName)
{
    m_tagName = tagName;
}

String XMLNode::ToString() const
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

const List<String> &XMLNode::GetAttributesOrderList() const
{
    return m_attributeOrder;
}

List<std::pair<String, XMLAttribute> > XMLNode::GetAttributesListInOrder() const
{
    List< std::pair<String, XMLAttribute> > attributes;
    for (const String& attrName : m_attributeOrder)
    {
        attributes.PushBack( std::make_pair(attrName, m_attributes[attrName]) );
    }
    return attributes;
}

const List<XMLNode> &XMLNode::GetChildren() const
{
    return m_children;
}

List<XMLNode> &XMLNode::GetChildren()
{
    return m_children;
}

XMLNode XMLNode::FromString(const String &xml)
{
    return XMLParser::FromString(xml);
}
