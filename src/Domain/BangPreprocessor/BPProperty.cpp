#include "BPProperty.h"

#include <regex>

#include "Bang/Debug.h"
#include "Bang/BangPreprocessor.h"

typedef BangPreprocessor BP;

BPProperty::BPProperty()
{

}

void BPProperty::FromString(String::Iterator propBegin,
                            String::Iterator propEnd,
                            BPProperty *outProperty,
                            bool *success)
{
    *success = false;

    String::Iterator propListBegin, propListEnd;
    BP::GetNextScope(propBegin, propEnd, &propListBegin, &propListEnd,
                     '(', ')');

    // Process property list
    String propertyListStr(propListBegin + 1, propListEnd - 1);
    Array<String> propertyList = propertyListStr.Split(',', true);
    if (propertyList.Size() == 0)
    {
        Debug_Error("BP Error: BP_PROPERTY has 0 properties,"
                    " but must have at least a name");
        return;
    }
    outProperty->m_propertyName = propertyList[0];

    // Skip modifiers
    String nextWord = "";
    String::Iterator wordEnd = propListEnd;
    do
    {
        String::Iterator wordBegin;
        BP::FindNextWord(wordEnd, propEnd, &wordBegin, &wordEnd);
        nextWord = String(wordBegin, wordEnd);
    }
    while ( BP::Modifiers.Contains(nextWord) );

    String variableType = nextWord;
    if (!BP::VarTypes.Contains(variableType))
    {
        Debug_Error("BP Error: Expected a variable type,"
                    "but got '" << variableType << "'");
        return;
    }

    outProperty->m_variableType = variableType;

    String::Iterator nameBegin, nameEnd;
    BP::FindNextWord(wordEnd, propEnd, &nameBegin, &nameEnd);
    if (nameBegin == propEnd || nameEnd == propEnd)
    {
        Debug_Error("BP Error: Expected a variable name");
        return;
    }
    outProperty->m_variableName = String(nameBegin, nameEnd);

    String::Iterator assignBegin = std::find(nameEnd, propEnd, '=');
    if (assignBegin != propEnd)
    {
        String initValue (assignBegin + 1, propEnd - 1);
        initValue = initValue.Trim({' ', '"'});
        outProperty->m_variableInitValue = initValue;
    }

    *success = true;
}

String BPProperty::GetInitializationCode(const String &propInitVarName) const
{
    const String vName = propInitVarName;

    String code = "";
    code += vName + ".SetPropertyName(\"" + GetPropertyName() + "\");\n";
    code += vName + ".SetVariableType(\"" + GetVariableType() + "\");\n";
    code += vName + ".SetVariableName(\"" + GetVariableName() + "\");\n";
    code += vName + ".SetVariableInitValue(\"" + GetVariableInitValue() + "\");\n";
    return code;
}

void BPProperty::SetPropertyName(const String &name)
{
    m_propertyName = name;
}

void BPProperty::SetVariableType(const String &varType)
{
    m_variableType = varType;
}

void BPProperty::SetVariableName(const String &varName)
{
    m_variableName = varName;
}

void BPProperty::SetVariableInitValue(const String &initValue)
{
    m_variableInitValue = initValue;
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

const String &BPProperty::GetVariableInitValue() const
{
    return m_variableInitValue;
}

String BPProperty::ToString() const
{
    return "(" + GetPropertyName() + ", " +
                 GetVariableType() + ", " +
                 GetVariableName() + " = " +
                 GetVariableInitValue() +
            ")";
}
