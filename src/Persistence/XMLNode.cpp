#include "XMLNode.h"

#include "XMLParser.h"

XMLNode::XMLNode(const std::string &tagName) :
    m_tagName(tagName)
{
}

void XMLNode::AddChild(XMLNode node)
{
    m_children.push_back(node);
}

void XMLNode::AddAttribute(const std::string &attributeName,
                           const std::string &attributeValue)
{
    m_attributes[attributeName] = attributeValue;
}

const std::string XMLNode::GetAttributeValue(const std::string &attributeName)
{
    if (m_attributes.find(attributeName) != m_attributes.end())
    {
        return m_attributes[attributeName];
    }
    return "";
}

std::string XMLNode::ToString(const std::string& indent) const
{
    std::string str = "";

    str += indent + "<" + m_tagName;
    for(auto itAttr : m_attributes)
    {
        str += ( " " + itAttr.first + "=\"" + itAttr.second + "\"");
    }
    str += ">\n";

    const std::string newIndent = indent + " ";
    for(const XMLNode &child : m_children)
    {
        str += indent + child.ToString(newIndent);
    }
    str += indent + "<" + m_tagName + "/>\n";
    return str;
}

void XMLNode::SetTagName(const std::string tagName)
{
    m_tagName = tagName;
}

const std::string &XMLNode::GetTagName() const
{
    return m_tagName;
}

const std::map<std::string, std::string> &XMLNode::GetAttributes() const
{
    return m_attributes;
}

const std::list<XMLNode>& XMLNode::GetChildren() const
{
    return m_children;
}
