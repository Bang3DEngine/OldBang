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

void XMLNode::UpdateAttributeValue(const std::string &attributeName,
                                   const std::string &newAttributeValue)
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

void XMLNode::SetGenericAttribute(const std::string &attributeName,
                                  const std::string &attributeValue,
                                  XMLAttribute::Type type,
                                  const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr(attributeName, attributeValue, type, properties);
    SetAttribute(attr);
}

void XMLNode::SetPointer(const std::string &attributeName, const void *value,
                         const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetPointer(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetBool(const std::string &attributeName, bool value,
                      const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetBool(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetInt(const std::string &attributeName, int value,
                     const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetInt(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetFloat(const std::string &attributeName, float value,
                       const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetFloat(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetVector2(const std::string &attributeName, const glm::vec2 &value,
                         const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetVector2(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetVector3(const std::string &attributeName, const Vector3 &value,
                         const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetVector3(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetVector4(const std::string &attributeName, const glm::vec4 &value,
                         const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr; attr.SetVector4(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetQuaternion(const std::string &attributeName, const Quaternion &value,
                            const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetQuaternion(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetRect(const std::string &attributeName, const Rect &value,
                      const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetRect(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetString(const std::string &attributeName, const std::string &value,
                        const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetString(value, properties);
    SetAttribute(attr);
}

void XMLNode::SetFilepath(const std::string &attributeName, const std::string &filepath,
                          const std::string &fileExtension,
                          const std::vector<XMLProperty>& properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetFilepath(filepath, fileExtension, properties);
    SetAttribute(attr);
}

void XMLNode::SetEnum(const std::string &attributeName, const std::vector<std::string>& enumNames,
                      int selectedEnumIndex, const std::vector<XMLProperty> &properties)
{
    XMLAttribute attr;
    attr.SetName(attributeName);
    attr.SetEnum(enumNames, selectedEnumIndex, properties);
    for (int i = 0; i < enumNames.size(); ++i)
    {
        const std::string &enumName = enumNames[i];
        XMLProperty prop("EnumName" + std::to_string(i), enumName);
        attr.SetProperty(prop);
    }

    SetAttribute(attr);
}

XMLAttribute* XMLNode::GetAttribute(const std::string &attributeName) const
{
    XMLAttribute *attr = nullptr;
    if (m_attributes.find(attributeName) != m_attributes.end())
    {
        attr = &(m_attributes[attributeName]);
    }
    return attr;
}

void XMLNode::RemoveAttribute(const std::string &attributeName)
{
    for (auto it = m_attributes.begin(); it != m_attributes.end(); ++it)
    {
        XMLAttribute &attr = it->second;
        if (attr.GetName() == attributeName)
        {
            it = m_attributes.erase(it);
        }
    }
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
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetBool() : false;
}

int XMLNode::GetInt(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetInt() : 0;
}

std::string XMLNode::GetString(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetString() : "";
}

std::string XMLNode::GetFilepath(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetFilepath() : "";
}

float XMLNode::GetFloat(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetFloat() : 0.0f;
}

glm::vec2 XMLNode::GetVector2(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetVector2() : glm::vec2(0);
}

Vector3 XMLNode::GetVector3(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetVector3() : Vector3::zero;
}

glm::vec4 XMLNode::GetVector4(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetVector4() : glm::vec4(0);
}

Quaternion XMLNode::GetQuaternion(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetQuaternion() : Quaternion();
}

Rect XMLNode::GetRect(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetRect() : Rect();
}

int XMLNode::GetEnumSelectedIndex(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetEnumSelectedIndex() : -1;
}

std::string XMLNode::GetEnumSelectedName(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetEnumSelectedName() : "";
}

std::vector<std::string> XMLNode::GetEnumNames(const std::string &attributeName) const
{
    XMLAttribute *attr = GetAttribute(attributeName);
    return attr ? attr->GetEnumNames() : std::vector<std::string>();
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

const std::string XMLNode::ToString(const std::string& indent) const
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
    str += indent + "</" + m_tagName + ">\n";
    return str;
}

void XMLNode::SetTagName(const std::string tagName)
{
    m_tagName = tagName;
}

const std::string XMLNode::ToString() const
{
    return ToString("");
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
