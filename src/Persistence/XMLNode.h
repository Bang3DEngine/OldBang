#ifndef XMLNODE_H
#define XMLNODE_H

#include "Bang.h"
#include <map>
#include <list>
#include <string>
#include <sstream>

#include "Rect.h"
#include "Vector3.h"
#include "Quaternion.h"

class XMLNode
{
private:
    std::string m_tagName = "";
    mutable std::map<std::string, std::string> m_attributes;
    std::list<XMLNode*> m_children;

public:
    XMLNode(const std::string &tagName = "");
    virtual ~XMLNode();

    void AddChild(XMLNode *node);
    void AddGenericAttribute(const std::string &attributeName,
                             const std::string &attributeValue);
    void AddAttribute(const std::string &attributeName,
                      void *value);
    void AddAttribute(const std::string &attributeName,
                      bool value);
    void AddAttribute(const std::string &attributeName,
                      int value);
    void AddAttribute(const std::string &attributeName,
                      float value);
    void AddAttribute(const std::string &attributeName,
                      const std::string &value);
    void AddAttribute(const std::string &attributeName,
                      const glm::vec2 &value);
    void AddAttribute(const std::string &attributeName,
                      const Vector3 &value);
    void AddAttribute(const std::string &attributeName,
                      const glm::vec4 &value);
    void AddAttribute(const std::string &attributeName,
                      const Quaternion &value);
    void AddAttribute(const std::string &attributeName,
                      const Rect &value);


    std::string GetAttributeValue(const std::string& attributeName) const;
    bool GetBool(const std::string& attributeName) const;
    int GetInt(const std::string& attributeName) const;
    float GetFloat(const std::string& attributeName) const;
    std::string GetString(const std::string& attributeName) const;
    glm::vec2 GetVector2(const std::string& attributeName) const;
    Vector3 GetVector3(const std::string& attributeName) const;
    glm::vec4 GetVector4(const std::string& attributeName) const;
    Quaternion GetQuaternion(const std::string& attributeName) const;
    Rect GetRect(const std::string& attributeName) const;

    const XMLNode *GetChild(const std::string &name) const;
    void SetTagName(const std::string tagName);
    std::string ToString(const std::string& indent = "") const;

    const std::string& GetTagName() const;
    const std::map<std::string, std::string>& GetAttributes() const;
    const std::list<XMLNode*>& GetChildren() const;
};

#endif // XMLNODE_H
