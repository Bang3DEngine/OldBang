#ifndef BPPROPERTY_H
#define BPPROPERTY_H

#include "Bang/String.h"
#include "Bang/IToString.h"

class BPProperty : public IToString
{
public:
    BPProperty();

    static void FromString(String::Iterator propBegin,
                           String::Iterator propEnd,
                           BPProperty *outProperty,
                           bool *success);

    String GetInitializationCode(const String &propInitVarName) const;

    void SetPropertyName(const String &name);
    void SetVariableType(const String &varType);
    void SetVariableName(const String &varName);
    void SetVariableInitValue(const String &initValue);

    const String& GetPropertyName() const;
    const String& GetVariableType() const;
    const String& GetVariableName() const;
    const String& GetVariableInitValue() const;

private:
    String m_propertyName = "";
    String m_variableType = "";
    String m_variableName = "";
    String m_variableInitValue = "";

    String ToString() const override;
};

#endif // BPPROPERTY_H
