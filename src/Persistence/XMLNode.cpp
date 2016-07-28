#include "XMLNode.h"

#include "XMLParser.h"

#include "FileReader.h"
#include "FileWriter.h"

XMLNode::XMLNode(const std::string &tagName) :
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

void XMLNode::AddChild(XMLNode *node)
{
    m_children.push_back(node);
}

void XMLNode::SetGenericAttribute(const std::string &attributeName,
                                  const std::string &attributeValue,
                                  XMLAttribute::Type type)
{
    XMLAttribute attr(attributeName, attributeValue, type);
    m_attributes[attributeName] = attr;
}

void XMLNode::SetAttribute(const std::string &attributeName, void *value)
{
    std::ostringstream oss;
    oss << value;
    SetGenericAttribute(attributeName, oss.str(), XMLAttribute::Type::String);
}

void XMLNode::SetAttribute(const std::string &attributeName, bool value)
{
     SetGenericAttribute(attributeName, value ? "true" : "false", XMLAttribute::Type::Bool);
}

void XMLNode::SetAttribute(const std::string &attributeName, int value)
{
    std::ostringstream oss;
    oss << value;
    SetGenericAttribute(attributeName, oss.str(), XMLAttribute::Type::Int);
}


void XMLNode::SetAttribute(const std::string &attributeName, const std::string &value)
{
    std::ostringstream oss;
    oss << value;
    SetGenericAttribute(attributeName, oss.str(), XMLAttribute::Type::String);
}

void XMLNode::SetAttribute(const std::string &attributeName, float value)
{
    std::ostringstream oss;
    oss << value;
    SetGenericAttribute(attributeName, oss.str(), XMLAttribute::Type::Float);
}

void XMLNode::SetAttribute(const std::string &attributeName, const glm::vec2 &value)
{
    std::ostringstream oss;
    oss << "(" << value.x << ", " <<
                  value.y << ")";
    SetGenericAttribute(attributeName, oss.str(), XMLAttribute::Type::Vector2);
}

void XMLNode::SetAttribute(const std::string &attributeName, const Vector3 &value)
{
    std::ostringstream oss;
    oss << "(" << value.x << ", " <<
                  value.y << ", " <<
                  value.z << ")";
    SetGenericAttribute(attributeName, oss.str(), XMLAttribute::Type::Vector3);
}

void XMLNode::SetAttribute(const std::string &attributeName, const glm::vec4 &value)
{
    std::ostringstream oss;
    oss << "(" << value.x << ", " <<
                  value.y << ", " <<
                  value.z << ", " <<
                  value.w << ")";
    SetGenericAttribute(attributeName, oss.str(), XMLAttribute::Type::Vector4);
}

void XMLNode::SetAttribute(const std::string &attributeName, const Quaternion &value)
{
    std::ostringstream oss;
    oss << "(" << value.w << ", " <<
                  value.x << ", " <<
                  value.y << ", " <<
                  value.z << ")";
    SetGenericAttribute(attributeName, oss.str(), XMLAttribute::Type::Quaternion);
}

void XMLNode::SetAttribute(const std::string &attributeName, const Rect &value)
{
    std::ostringstream oss;
    oss << "(" << value.m_minx << ", " <<
                  value.m_miny << ", " <<
                  value.m_maxx << ", " <<
                  value.m_maxy << ")";
    SetGenericAttribute(attributeName, oss.str(), XMLAttribute::Type::Rect);
}

std::string XMLNode::GetAttributeValue(const std::string &attributeName) const
{
    if (m_attributes.find(attributeName) != m_attributes.end())
    {
        return m_attributes[attributeName].GetValue();
    }
    return "";
}

bool XMLNode::GetBool(const std::string &attributeName) const
{
    if (m_attributes.find(attributeName) != m_attributes.end())
    {
        std::string value = m_attributes[attributeName].GetValue();
        return value == "true";
    }
    return "";
}

int XMLNode::GetInt(const std::string &attributeName) const
{
    if (m_attributes.find(attributeName) != m_attributes.end())
    {
        std::string value = m_attributes[attributeName].GetValue();
        std::istringstream iss(value);
        int v; iss >> v;
        return v;
    }
    return -1;
}

float XMLNode::GetFloat(const std::string &attributeName) const
{
    if (m_attributes.find(attributeName) != m_attributes.end())
    {
        std::string value = m_attributes[attributeName].GetValue();
        return StringUtils::ToFloat(value);
    }
    return -1.0f;
}

std::string XMLNode::GetFilepath(const std::string &attributeName) const
{
    return GetString(attributeName);
}

std::string XMLNode::GetString(const std::string &attributeName) const
{
    if  (m_attributes.find(attributeName) != m_attributes.end())
    {
        return m_attributes[attributeName].GetValue();
    }
    return "";
}

glm::vec2 XMLNode::GetVector2(const std::string &attributeName) const
{
    float x = 0, y = 0;
    if  (m_attributes.find(attributeName) != m_attributes.end())
    {
        std::string value = m_attributes[attributeName].GetValue();
        std::string insidePars = StringUtils::Split(value, '(')[1];
        insidePars =  StringUtils::Split(insidePars, ')')[0];
        std::vector<std::string> numbers = StringUtils::Split(insidePars, ',');
        x = StringUtils::ToFloat(numbers[0]);
        y = StringUtils::ToFloat(numbers[1]);
    }
    return glm::vec2(x,y);
}

Vector3 XMLNode::GetVector3(const std::string &attributeName) const
{
    float x = 0, y = 0, z = 0;
    if  (m_attributes.find(attributeName) != m_attributes.end())
    {
        std::string value = m_attributes[attributeName].GetValue();
        std::string insidePars = StringUtils::Split(value, '(')[1];
        insidePars =  StringUtils::Split(insidePars, ')')[0];
        std::vector<std::string> numbers = StringUtils::Split(insidePars, ',');
        x = StringUtils::ToFloat(numbers[0]);
        y = StringUtils::ToFloat(numbers[1]);
        z = StringUtils::ToFloat(numbers[2]);
    }
    return Vector3(x,y,z);
}

glm::vec4 XMLNode::GetVector4(const std::string &attributeName) const
{
    float x = 0, y = 0, z = 0, w = 0;
    if  (m_attributes.find(attributeName) != m_attributes.end())
    {
        std::string value = m_attributes[attributeName].GetValue();
        std::string insidePars = StringUtils::Split(value, '(')[1];
        insidePars =  StringUtils::Split(insidePars, ')')[0];
        std::vector<std::string> numbers = StringUtils::Split(insidePars, ',');
        x = StringUtils::ToFloat(numbers[0]);
        y = StringUtils::ToFloat(numbers[1]);
        z = StringUtils::ToFloat(numbers[2]);
        w = StringUtils::ToFloat(numbers[3]);
    }
    return glm::vec4(x,y,z,w);
}

Quaternion XMLNode::GetQuaternion(const std::string &attributeName) const
{
    glm::vec4 v = GetVector4(attributeName);
    return Quaternion(v.w, v.x, v.y, v.z);
}

Rect XMLNode::GetRect(const std::string &attributeName) const
{
    glm::vec4 v = GetVector4(attributeName);
    return Rect(v.x, v.y, v.z, v.w);
}

const XMLNode *XMLNode::GetChild(const std::string &name) const
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

std::string XMLNode::ToString(const std::string& indent) const
{
    std::string str = "";

    str += indent + "<" + m_tagName;
    for(auto itAttr : m_attributes)
    {
        XMLAttribute attr = itAttr.second;
        str += " " + attr.ToString() + "\n";
        for (int i = 0; i < m_tagName.length() + indent.length() + 1; ++i )
        {
            str += " ";
        }
    }
    str += ">\n";

    const std::string newIndent = indent + "    ";
    for(XMLNode *child : m_children)
    {
        str += child->ToString(newIndent);
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

const std::map<std::string, XMLAttribute> &XMLNode::GetAttributes() const
{
    return m_attributes;
}

const std::list<XMLNode*>& XMLNode::GetChildren() const
{
    return m_children;
}
