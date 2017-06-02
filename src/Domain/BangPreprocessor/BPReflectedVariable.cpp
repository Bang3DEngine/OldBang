#include "BPReflectedVariable.h"

#include <regex>

#include "Bang/List.h"
#include "Bang/BangPreprocessor.h"

typedef BangPreprocessor BP;

BPReflectedVariable::BPReflectedVariable()
{

}

void BPReflectedVariable::FromString(String::Iterator propBegin,
                            String::Iterator propEnd,
                            BPReflectedVariable *outProperty,
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
        std::cerr << "BP Error: BP_PROPERTY has 0 properties,"
                     " but must have at least a name" << std::endl;
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
        std::cerr << "BP Error: Expected a variable type,"
                     "but got '" << variableType << "'" << std::endl;
        return;
    }

    outProperty->m_variableType = variableType;

    String::Iterator nameBegin, nameEnd;
    BP::FindNextWord(wordEnd, propEnd, &nameBegin, &nameEnd);
    if (nameBegin == propEnd || nameEnd == propEnd)
    {
        std::cerr << "BP Error: Expected a variable name" << std::endl;
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

String BPReflectedVariable::GetInitializationCode(const String &propInitVarName) const
{
    const String vName = propInitVarName;

    String code = "";
    code += vName + ".SetPropertyName(\"" + GetPropertyName() + "\");\n";
    code += vName + ".SetVariableType(\"" + GetVariableType() + "\");\n";
    code += vName + ".SetVariableName(\"" + GetVariableName() + "\");\n";
    code += vName + ".SetVariableInitValue(\"" + GetVariableInitValue() + "\");\n";
    return code;
}

void BPReflectedVariable::SetPropertyName(const String &name)
{
    m_propertyName = name;
}

void BPReflectedVariable::SetVariableType(const String &varType)
{
    m_variableType = varType;
}

void BPReflectedVariable::SetVariableName(const String &varName)
{
    m_variableName = varName;
}

void BPReflectedVariable::SetVariableInitValue(const String &initValue)
{
    m_variableInitValue = initValue;
}

const String &BPReflectedVariable::GetPropertyName() const
{
    return m_propertyName;
}

const String &BPReflectedVariable::GetVariableType() const
{
    return m_variableType;
}

const String &BPReflectedVariable::GetVariableName() const
{
    return m_variableName;
}

const String &BPReflectedVariable::GetVariableInitValue() const
{
    return m_variableInitValue;
}

String BPReflectedVariable::ToString() const
{
    return "(" + GetPropertyName() + ", " +
                 GetVariableType() + ", " +
                 GetVariableName() + " = " +
                 GetVariableInitValue() +
            ")";
}
