#include "Bang/BPReflectedVariable.h"

#include <regex>

#include "Bang/List.h"
#include "Bang/BangPreprocessor.h"

using BP = BangPreprocessor;

BPReflectedVariable::BPReflectedVariable()
{

}

void BPReflectedVariable::FromString(
                            String::Iterator propBegin,
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
    Array<String> propertyList = propertyListStr.Split<Array>(',', true);
    if (propertyList.Size() == 0)
    {
        std::cerr << "BP Error: BP_REFLECT_VARIABLE has 0 properties,"
                     " but must have at least a name" << std::endl;
        return;
    }
    outProperty->m_name = propertyList[0];

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

String BPReflectedVariable::GetInitializationCode(const String &rvarInitVarName) const
{
    String src = R"VERBATIM(
            RVAR_VARIABLE_NAME.SetName("RVAR_NAME");
            RVAR_VARIABLE_NAME.SetVariableType("VARIABLE_TYPE");
            RVAR_VARIABLE_NAME.SetVariableName("VARIABLE_NAME");
            RVAR_VARIABLE_NAME.SetVariableInitValue("VARIABLE_INIT_VALUE");
    )VERBATIM";
    src.ReplaceInSitu("RVAR_VARIABLE_NAME",  rvarInitVarName);
    src.ReplaceInSitu("RVAR_NAME",           GetName());
    src.ReplaceInSitu("VARIABLE_TYPE",       GetVariableType());
    src.ReplaceInSitu("VARIABLE_NAME",       GetVariableName());
    src.ReplaceInSitu("VARIABLE_INIT_VALUE", GetVariableInitValue());
    return src;
}

bool BPReflectedVariable::IsInt() const
{
    return BP::VarTypeInt.Contains(GetVariableType());
}

bool BPReflectedVariable::IsFloat() const
{
    return BP::VarTypeFloat.Contains(GetVariableType());
}

bool BPReflectedVariable::IsDouble() const
{
    return BP::VarTypeDouble.Contains(GetVariableType());
}

bool BPReflectedVariable::IsString() const
{
    return BP::VarTypeString.Contains(GetVariableType());
}

void BPReflectedVariable::SetName(const String &name)
{
    m_name = name;
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

const String &BPReflectedVariable::GetName() const
{
    return m_name;
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
    return "(" + GetName() + ", " +
                 GetVariableType() + ", " +
                 GetVariableName() + " = " +
                 GetVariableInitValue() +
            ")";
}
