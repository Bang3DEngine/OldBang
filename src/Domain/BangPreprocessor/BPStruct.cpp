#include "BPStruct.h"

#include <regex>

#include "Bang/Debug.h"
#include "Bang/BangPreprocessor.h"

typedef BangPreprocessor BP;

BPStruct::BPStruct()
{
}

BPStruct::~BPStruct()
{
}

void BPStruct::FromString(String::Iterator structBegin,
                          String::Iterator structEnd,
                          BPStruct *outStruct,
                          bool *success)
{
    *success = true;

    // Parse the struct/class property
    String::Iterator structPropertyBegin = structBegin;

    String::Iterator propertyListBegin = std::find(structPropertyBegin,
                                                   structEnd, '(');
    String::Iterator propertyListEnd = std::find(structPropertyBegin,
                                                 structEnd, ')') + 1;

    String propertyListStr(propertyListBegin, propertyListEnd);
    Array<String> propertyList = propertyListStr.Split(',', true);
    if (propertyList.Size() < 0)
    {
        *success = false;
        Debug_Error("Error parsing class");
    }
    outStruct->m_structName = propertyList[0];

    // Find and skip "class"/"struct"
    String::Iterator structKeywordBegin = BP::Find(propertyListEnd + 1,
                                                   structEnd,
                                                   {"class", "struct"});
    String::Iterator structKeywordEnd = (structKeywordBegin + 1);
    BP::SkipUntilNewBlank(&structKeywordEnd, structEnd);

    // Get class/struct variable name (XXX in  "class XXX { ... }")
    String::Iterator structVarNameBegin = structKeywordEnd;
    BP::SkipBlanks(&structVarNameBegin, structEnd);
    String::Iterator structVarNameEnd = structVarNameBegin;
    BP::SkipUntilNewBlank(&structVarNameEnd, structEnd);
    outStruct->m_structVariableName = String(structVarNameBegin,
                                             structVarNameEnd);
}

const String &BPStruct::GetStructName() const
{
    return m_structName;
}

const String &BPStruct::GetStructVariableName() const
{
    return m_structVariableName;
}

const Array<BPProperty> &BPStruct::GetProperties() const
{
    return m_properties;
}

String BPStruct::ToString() const
{
    return GetStructName() + " " +
           GetStructVariableName() + " " +
           GetProperties().ToString();
}
