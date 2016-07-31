#ifndef XMLPROPERTY_H
#define XMLPROPERTY_H

#include <string>

#include  "StringUtils.h"

class XMLProperty
{
private:
    std::string m_name = "";
    std::string m_value = "";

public:
    const static XMLProperty Hidden;
    const static XMLProperty Inline;
    const static XMLProperty Readonly;
    const static XMLProperty FileExtension;

    XMLProperty();
    XMLProperty(const std::string &name, const std::string &value = "");

    void SetName(const std::string &name);
    void SetValue(const std::string &value);

    const std::string& GetName() const;
    const std::string& GetValue() const;

    static XMLProperty FromString(const std::string &string);
};

#endif // XMLPROPERTY_H
