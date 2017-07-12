#ifndef XMLPROPERTY_H
#define XMLPROPERTY_H

#include "Bang/String.h"
#include "Bang/IToString.h"

class XMLProperty : public IToString
{
public:
    const static XMLProperty IsEngineFile;

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
