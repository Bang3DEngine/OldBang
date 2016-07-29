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
#include "StringUtils.h"
#include "XMLAttribute.h"
#include "IToString.h"

class XMLNode : public IToString
{
private:
    std::string m_tagName = "";
    mutable std::map<std::string, XMLAttribute> m_attributes;
    std::list<XMLNode*> m_children;

public:
    XMLNode(const std::string &tagName = "");
    virtual ~XMLNode();

    void AddChild(XMLNode *node);
    void SetGenericAttribute(const std::string &attributeName,
                             const std::string &attributeValue,
                             XMLAttribute::Type type,
                             const std::vector<std::string>& properties = {});
    void SetPointer(const std::string &attributeName,
                    const void *value,
                    const std::vector<std::string>& properties = {});
    void SetBool(const std::string &attributeName,
                 bool value,
                 const std::vector<std::string>& properties = {});
    void SetInt(const std::string &attributeName,
                int value,
                const std::vector<std::string>& properties = {});
    void SetFloat(const std::string &attributeName,
                  float value,
                  const std::vector<std::string>& properties = {});
    void SetString(const std::string &attributeName,
                   const std::string &value,
                   const std::vector<std::string>& properties = {});
    void SetVector2(const std::string &attributeName,
                    const glm::vec2 &value,
                    const std::vector<std::string>& properties = {});
    void SetVector3(const std::string &attributeName,
                    const Vector3 &value,
                    const std::vector<std::string>& properties = {});
    void SetVector4(const std::string &attributeName,
                    const glm::vec4 &value,
                    const std::vector<std::string>& properties = {});
    void SetQuaternion(const std::string &attributeName,
                       const Quaternion &value,
                       const std::vector<std::string>& properties = {});
    void SetRect(const std::string &attributeName,
                 const Rect &value,
                 const std::vector<std::string>& properties = {});
    void SetFilepath(const std::string &attributeName,
                     const std::string &filepath,
                     const std::vector<std::string>& properties = {});


    std::string GetAttributeValue(const std::string& attributeName) const;
    bool GetBool(const std::string& attributeName) const;
    int GetInt(const std::string& attributeName) const;
    float GetFloat(const std::string& attributeName) const;
    std::string GetFilepath(const std::string& attributeName) const;
    std::string GetString(const std::string& attributeName) const;
    glm::vec2 GetVector2(const std::string& attributeName) const;
    Vector3 GetVector3(const std::string& attributeName) const;
    glm::vec4 GetVector4(const std::string& attributeName) const;
    Quaternion GetQuaternion(const std::string& attributeName) const;
    Rect GetRect(const std::string& attributeName) const;

    const XMLNode *GetChild(const std::string &name) const;
    void SetTagName(const std::string tagName);
    const std::string ToString() const override;
    const std::string ToString(const std::string& indent) const;

    const std::string& GetTagName() const;
    const std::map<std::string, XMLAttribute>& GetAttributes() const;
    const std::list<XMLNode*>& GetChildren() const;
};

#endif // XMLNODE_H
