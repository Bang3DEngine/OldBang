#include "BPReflectedStruct.h"

#include <regex>

#include "Bang/BangPreprocessor.h"

typedef BangPreprocessor BP;

BPReflectedStruct::BPReflectedStruct()
{
}

BPReflectedStruct::~BPReflectedStruct()
{
}

void BPReflectedStruct::FromString(String::Iterator structBegin,
                          String::Iterator structEnd,
                          BPReflectedStruct *outStruct,
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
    if (propertyList.Size() == 0)
    {
        std::cerr << "BP Error: BP_CLASS has 0 properties, but must have at"
                     "least a name" << std::endl;
        return;
    }
    outStruct->SetStructName(propertyList[0]);

    // Find and skip "class"/"struct"
    String::Iterator structKeywordBegin, structKeywordEnd;
    BP::FindNextWord(structPropertyListEnd, structEnd,
                     &structKeywordBegin, &structKeywordEnd);
    String keyword(structKeywordBegin, structKeywordEnd);
    if (keyword != "class" && keyword != "struct")
    {
        std::cerr << "BP Error: 'class' or 'struct' keyword expected after"
                     " BANG_CLASS(...)" << std::endl;
        return;
    }

    // Get class/struct variable name (XXX in  "class XXX { ... }")
    String::Iterator structVarNameBegin, structVarNameEnd;
    BP::FindNextWord(structKeywordEnd, structEnd,
                     &structVarNameBegin, &structVarNameEnd);
    outStruct->SetStructVariableName(String(structVarNameBegin,
                                            structVarNameEnd));

    String::Iterator it = structVarNameEnd;
    while (it != structEnd)
    {
        String::Iterator propertyBegin = BP::Find(it, structEnd,
                                                  BP::RVariablePrefixes);
        if (propertyBegin == structEnd) { break; }

        String::Iterator propertyEnd = propertyBegin;
        BP::SkipUntilNext(&propertyEnd, structEnd, {";"});
        if (propertyEnd == structEnd) { break; }
        propertyEnd += 1;

        BPReflectedVariable bProperty;
        BPReflectedVariable::FromString(propertyBegin, propertyEnd,
                               &bProperty, success);
        outStruct->AddProperty(bProperty);

        it = propertyEnd;
    }

    *success = true;
}

void BPReflectedStruct::SetStructName(const String &structName)
{
    m_structName = structName;
}

void BPReflectedStruct::SetStructVariableName(const String &structVarName)
{
    m_structVariableName = structVarName;
}

void BPReflectedStruct::AddProperty(const BPReflectedVariable &prop)
{
    m_properties.Add(prop);
}

String BPReflectedStruct::GetInitializationCode(const String &structInitVarName) const
{
    const String vName = structInitVarName;

    int i = 0;
    String code = "";
    for (const BPReflectedVariable &prop : GetProperties())
    {
        String propVarName = "prop" + String(i);
        code += "BPReflectedVariable " + propVarName + ";\n";
        code += prop.GetInitializationCode(propVarName);
        code += vName + ".AddProperty(" + propVarName + ");\n";
        ++i;
    }
    return code;
}

const String &BPReflectedStruct::GetStructName() const
{
    return m_structName;
}

const String &BPReflectedStruct::GetStructVariableName() const
{
    return m_structVariableName;
}

const Array<BPReflectedVariable> &BPReflectedStruct::GetProperties() const
{
    return m_properties;
}

String BPReflectedStruct::ToString() const
{
    return "{ " +
           GetStructName() + ", " +
           GetStructVariableName() + ", " +
           GetProperties().ToString() +
           " }";
}
