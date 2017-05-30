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
    *success = false;

    // Parse the struct/class property
    String::Iterator structPropertyListBegin, structPropertyListEnd;
    BP::GetNextScope(structBegin, structEnd,
                     &structPropertyListBegin, &structPropertyListEnd,
                     '(', ')');

    String propertyListStr(structPropertyListBegin + 1,
                           structPropertyListEnd - 1);
    Array<String> propertyList = propertyListStr.Split(',', true);
    if (propertyList.Size() < 0)
    {
        Debug_Error("BP Error: BANG_CLASS has 0 properties, but must have at"
                    "least a name");
        return;
    }
    outStruct->m_structName = propertyList[0];

    // Find and skip "class"/"struct"
    String::Iterator structKeywordBegin, structKeywordEnd;
    BP::FindNextWord(structPropertyListEnd, structEnd,
                     &structKeywordBegin, &structKeywordEnd);
    String keyword(structKeywordBegin, structKeywordEnd);
    if (keyword != "class" && keyword != "struct")
    {
        Debug_Error("BP Error: 'class' or 'struct' keyword expected after"
                    " BANG_CLASS(...)");
        return;
    }

    // Get class/struct variable name (XXX in  "class XXX { ... }")
    String::Iterator structVarNameBegin, structVarNameEnd;
    BP::FindNextWord(structKeywordEnd, structEnd,
                     &structVarNameBegin, &structVarNameEnd);
    outStruct->m_structVariableName = String(structVarNameBegin,
                                             structVarNameEnd);

    String::Iterator it = structVarNameEnd;
    while (it != structEnd)
    {
        String::Iterator propertyBegin = BP::Find(it, structEnd,
                                                  BP::PropertyPrefixes);
        if (propertyBegin == structEnd) { break; }

        String::Iterator propertyEnd = propertyBegin;
        BP::SkipUntilNext(&propertyEnd, structEnd, {";"});
        if (propertyEnd == structEnd) { break; }
        propertyEnd += 1;

        BPProperty bProperty;
        BPProperty::FromString(propertyBegin, propertyEnd,
                               &bProperty, success);
        outStruct->m_properties.Add(bProperty);

        it = propertyEnd;
    }

    *success = true;
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
    return "{ " +
           GetStructName() + ", " +
           GetStructVariableName() + ", " +
           GetProperties().ToString() +
           " }";
}
