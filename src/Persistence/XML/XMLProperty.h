#ifndef XMLPROPERTY_H
#define XMLPROPERTY_H

#include "Bang/String.h"
#include "Bang/IToString.h"

class XMLProperty : public IToString
{
public:
    const static XMLProperty Hidden;
    const static XMLProperty Inline;
    const static XMLProperty BigText;
    const static XMLProperty Readonly;
    const static XMLProperty Disabled;
    const static XMLProperty IsEngineFile;
    const static XMLProperty FileExtension;
    const static XMLProperty DontWriteToFile;

    XMLProperty();
    XMLProperty(const String &name, const String &value = "");

    void SetName(const String &name);
    void SetValue(const String &value);

    const String& GetName() const;
    const String& GetValue() const;

    static XMLProperty FromString(const String &string);
    String ToString() const override;

    bool operator==(const XMLProperty &rhs) const;

private:
    String m_name = "";
    String m_value = "";
};

#endif // XMLPROPERTY_H
