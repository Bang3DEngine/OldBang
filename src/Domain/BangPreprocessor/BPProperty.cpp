#include "BPProperty.h"

#include <regex>

#include "Bang/Debug.h"
#include "Bang/BangPreprocessor.h"

typedef BangPreprocessor BP;

BPProperty::BPProperty()
{

}

void BPProperty::FromString(const String &propString,
                            BPProperty *outProperty,
                            bool *success)
{
    *success = true;
}

const String &BPProperty::GetPropertyName() const
{
    return m_propertyName;
}

const String &BPProperty::GetVariableType() const
{
    return m_variableType;
}

const String &BPProperty::GetVariableName() const
{
    return m_variableName;
}

String BPProperty::ToString() const
{
    return "(" + GetPropertyName() + "," +
                 GetVariableType() + "," +
                 GetVariableName() +
            ")";
}
