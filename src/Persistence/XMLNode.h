#ifndef XMLNODE_H
#define XMLNODE_H

#include <map>
#include <list>
#include <string>
#include <sstream>

#include "FileReader.h"
#include "FileWriter.h"

class XMLNode
{
private:
    std::string m_tagName = "";
    mutable std::map<std::string, std::string> m_attributes;
    std::list<XMLNode*> m_children;

public:
    XMLNode(const std::string &tagName = "");

    void AddChild(XMLNode *node);
    void AddAttribute(const std::string &attributeName,
                      const std::string &attributeValue);
    const std::string GetAttributeValue(const std::string& attributeName) const;
    const XMLNode *GetChild(const std::string &name) const;
    void SetTagName(const std::string tagName);
    std::string ToString(const std::string& indent = "") const;

    const std::string& GetTagName() const;
    const std::map<std::string, std::string>& GetAttributes() const;
    const std::list<XMLNode*>& GetChildren() const;
};

#endif // XMLNODE_H
