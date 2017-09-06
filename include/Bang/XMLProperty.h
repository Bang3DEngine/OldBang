#ifndef XMLPROPERTY_H
#define XMLPROPERTY_H

#include "Bang/String.h"
#include "Bang/IToString.h"

NAMESPACE_BANG_BEGIN

class XMLProperty : public IToString
{
public:
    const static XMLProperty IsEngineFile;

    XMLProperty();
    XMLProperty(const String &name, const String &value = "");
    virtual ~XMLProperty();

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

NAMESPACE_BANG_END

#endif // XMLPROPERTY_H
