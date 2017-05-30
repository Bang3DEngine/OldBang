#ifndef BPSTRUCT_H
#define BPSTRUCT_H

#include "Bang/Array.h"
#include "Bang/String.h"
#include "Bang/IToString.h"
#include "Bang/BPProperty.h"

class BPStruct : public IToString
{
public:
    BPStruct();
    virtual ~BPStruct();

    static void FromString(String::Iterator structBegin,
                           String::Iterator structEnd,
                           BPStruct *outStruct,
                           bool *success);

    const String &GetStructName() const;
    const String &GetStructVariableName() const;
    const Array<BPProperty> &GetProperties() const;

    String ToString() const override;

private:
    String m_structName = "";
    String m_structVariableName = "";
    Array<BPProperty> m_properties;
};

#endif // BPSTRUCT_H
