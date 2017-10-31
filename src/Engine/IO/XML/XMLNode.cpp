#include "Bang/XMLNode.h"

#include "Bang/Paths.h"
#include "Bang/XMLParser.h"

USING_NAMESPACE_BANG

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

bool XMLNode::Contains(const String &attrName) const
{
    return m_attributes.ContainsKey(attrName);
}

void XMLNode::Set(const XMLAttribute &attribute)
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

void XMLNode::Set(const String &attributeName,
                  const String &attributeValue,
                  const Array<XMLProperty>& properties)
{
    XMLAttribute attr(attributeName, attributeValue, properties);
    Set(attr);
}

void XMLNode::RemoveAttribute(const String &attributeName)
{
    for (auto it = m_attributes.Begin(); it != m_attributes.End();)
    {
        const XMLAttribute &attr = it->second;
        if (attr.GetName() == attributeName)
        {
            m_attributes.Remove(it++);
            m_attributeOrder.Remove(attr.GetName());
        }
        else { ++it; }
    }
}

XMLAttribute *XMLNode::GetAttribute(const String &attributeName) const
{
    for (auto& itPair : m_attributes)
    {
        XMLAttribute &attr = itPair.second;
        if (attr.GetName() == attributeName) { return &attr; }
    }
    return nullptr;
}

String XMLNode::GetAttributeValue(const String &attributeName) const
{
    if (m_attributes.ContainsKey(attributeName))
    {
        return m_attributes[attributeName].GetStringValue();
    }
    return "";
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
        const XMLAttribute* attr = itAttr.second;
        str += " " + attr->ToString() + "\n";
        for (int i = 0; i < m_tagName.Size() + indent.Size() + 1; ++i )
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

List<std::pair<String, XMLAttribute*> > XMLNode::GetAttributesListInOrder() const
{
    List< std::pair<String, XMLAttribute*> > attributes;
    for (const String& attrName : m_attributeOrder)
    {
        attributes.PushBack( std::make_pair(attrName, &m_attributes[attrName]) );
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
