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

void XMLNode::AddGenericAttribute(const std::string &attributeName,
                           const std::string &attributeValue)
{
    m_attributes[attributeName] = attributeValue;
}

void XMLNode::AddAttribute(const std::string &attributeName, void *value)
{
    std::ostringstream oss;
    oss << value;
    m_attributes[attributeName] = oss.str();
}

void XMLNode::AddAttribute(const std::string &attributeName, bool value)
{
     AddGenericAttribute(attributeName,
                         value ? "true" : "false");
}

void XMLNode::AddAttribute(const std::string &attributeName, int value)
{
    std::ostringstream oss;
    oss << value;
    AddGenericAttribute(attributeName, oss.str());
}

void XMLNode::AddAttribute(const std::string &attributeName, float value)
{
    std::ostringstream oss;
    oss << value;
    AddGenericAttribute(attributeName, oss.str());
}

void XMLNode::AddAttribute(const std::string &attributeName, const std::string &value)
{
    std::ostringstream oss;
    oss << value;
    AddGenericAttribute(attributeName, oss.str());
}

void XMLNode::AddAttribute(const std::string &attributeName, const glm::vec2 &value)
{
    std::ostringstream oss;
    oss << "Vector2(" << value.x << ", " <<
                         value.y << ")";
    AddGenericAttribute(attributeName, oss.str());
}

void XMLNode::AddAttribute(const std::string &attributeName, const Vector3 &value)
{
    std::ostringstream oss;
    oss << "Vector3(" << value.x << ", " <<
                         value.y << ", " <<
                         value.z << ")";
    AddGenericAttribute(attributeName, oss.str());
}

void XMLNode::AddAttribute(const std::string &attributeName, const glm::vec4 &value)
{
    std::ostringstream oss;
    oss << "Vector4(" << value.x << ", " <<
                         value.y << ", " <<
                         value.z << ", " <<
                         value.w << ")";
    AddGenericAttribute(attributeName, oss.str());
}

void XMLNode::AddAttribute(const std::string &attributeName, const Quaternion &value)
{
    std::ostringstream oss;
    oss << "Quaternion(" << value.x << ", " <<
                            value.y << ", " <<
                            value.z << ", " <<
                            value.w << ")";
    AddGenericAttribute(attributeName, oss.str());
}

void XMLNode::AddAttribute(const std::string &attributeName, const Rect &value)
{
    std::ostringstream oss;
    oss << "Rect(" << value.m_minx << ", " <<
                      value.m_miny << ", " <<
                      value.m_maxx << ", " <<
                      value.m_maxy << ")";
    AddGenericAttribute(attributeName, oss.str());
}

std::string XMLNode::GetAttributeValue(const std::string &attributeName) const
{
    if (m_attributes.find(attributeName) != m_attributes.end())
    {
        return m_attributes[attributeName];
    }
    return "";
}

bool XMLNode::GetBool(const std::string &attributeName) const
{
    if (m_attributes.find(attributeName) != m_attributes.end())
    {
        std::string value = m_attributes[attributeName];
        return value == "true";
    }
    return "";
}

int XMLNode::GetInt(const std::string &attributeName) const
{
    if (m_attributes.find(attributeName) != m_attributes.end())
    {
        std::string value = m_attributes[attributeName];
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
        std::string value = m_attributes[attributeName];
        std::istringstream iss(value);
        float v; iss >> v;
        return v;
    }
    return -1.0f;
}

std::string XMLNode::GetString(const std::string &attributeName) const
{
    return "";
}

glm::vec2 XMLNode::GetVector2(const std::string &attributeName) const
{
    /*
    std::string value = m_attributes[attributeName];
    int start = value.find_first_of('(', 0);

    start = value.find_first_not_of(XMLParser::TOKEN_SPACE, 0);
    int end = value.find_first_not_of("123456789.", start + 1);
    std::string strx =

    std::istringstream issx(strx);
    float x; issx >> x;

    std::istringstream issy(stry);
    float y; issy >> y;

    return glm::vec2(x,y);
    */

    return glm::vec2(0);
}

Vector3 XMLNode::GetVector3(const std::string &attributeName) const
{
    return Vector3(0);
}

glm::vec4 XMLNode::GetVector4(const std::string &attributeName) const
{
    return glm::vec4(0);
}

Quaternion XMLNode::GetQuaternion(const std::string &attributeName) const
{
    return Quaternion();
}

Rect XMLNode::GetRect(const std::string &attributeName) const
{
    return Rect(0,0,0,0);
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
        str += ( " " + itAttr.first + "=\"" + itAttr.second + "\"\n");
        for (int i = 0; i < m_tagName.length() + indent.length() + 1; ++i )
        {
            str += " ";
        }
    }
    str += ">\n";

    const std::string newIndent = indent + "  ";
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

const std::map<std::string, std::string> &XMLNode::GetAttributes() const
{
    return m_attributes;
}

const std::list<XMLNode*>& XMLNode::GetChildren() const
{
    return m_children;
}
