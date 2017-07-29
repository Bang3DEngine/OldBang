#include "Bang/IReflectable.h"

#include "Bang/BPReflectedStruct.h"

const BPReflectedStruct &IReflectable::GetReflectionInfo() const
{
    static BPReflectedStruct empty;
    return empty;
}
