#ifndef BPSTRUCT_H
#define BPSTRUCT_H

#include "Bang/Array.h"
#include "Bang/String.h"
#include "Bang/IToString.h"
#include "Bang/BPReflectedVariable.h"

class BPReflectedStruct : public IToString
{
public:
    BPReflectedStruct();
    virtual ~BPReflectedStruct();

    static void FromString(String::Iterator structBegin,
                           String::Iterator structEnd,
                           BPReflectedStruct *outStruct,
                           bool *success);

    void SetStructName(const String &structName);
    void SetStructVariableName(const String &structVarName);
    void AddProperty(const BPReflectedVariable &prop);

    String GetInitializationCode(const String &structInitVarName) const;

    const String &GetStructName() const;
    const String &GetStructVariableName() const;
    const Array<BPReflectedVariable> &GetProperties() const;

    String ToString() const override;

private:
    String m_structName = "";
    String m_structVariableName = "";
    Array<BPReflectedVariable> m_properties;
};

#endif // BPSTRUCT_H
