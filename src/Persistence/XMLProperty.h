#ifndef XMLPROPERTY_H
#define XMLPROPERTY_H



#include  "StringUtils.h"

class XMLProperty
{
private:
    String m_name = "";
    String m_value = "";

public:
    const static XMLProperty Hidden;
    const static XMLProperty Inline;
    const static XMLProperty BigText;
    const static XMLProperty Readonly;
    const static XMLProperty Disabled;
    const static XMLProperty FileExtension;

    XMLProperty();
    XMLProperty(const String &name, const String &value = "");

    void SetName(const String &name);
    void SetValue(const String &value);

    const String& GetName() const;
    const String& GetValue() const;

    static XMLProperty FromString(const String &string);
};

#endif // XMLPROPERTY_H
