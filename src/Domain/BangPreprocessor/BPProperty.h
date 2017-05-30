#ifndef BPPROPERTY_H
#define BPPROPERTY_H

#include "Bang/String.h"
#include "Bang/IToString.h"

class BPProperty : public IToString
{
public:
    BPProperty();

    static void FromString(const String &propString,
                           BPProperty *outProperty,
                           bool *success);

    const String& GetPropertyName() const;
    const String& GetVariableType() const;
    const String& GetVariableName() const;

private:
    String m_propertyName = "";
    String m_variableType = "";
    String m_variableName = "";

    String ToString() const override;
};

#endif // BPPROPERTY_H
