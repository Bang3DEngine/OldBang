#include "Bang/IReflectable.h"

#include "Bang/BPReflectedStruct.h"

USING_NAMESPACE_BANG

const BPReflectedStruct &IReflectable::GetReflectionInfo() const
{
    static BPReflectedStruct empty;
    return empty;
}
