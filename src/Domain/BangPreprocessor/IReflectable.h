#ifndef IREFLECTABLE_H
#define IREFLECTABLE_H

#include "Bang/BPReflectedStruct.h"

#define BP_REFLECT_VARIABLE(args...)
#define BP_REFLECT_CLASS(args...)
#define BP_REFLECT_STRUCT(args...)
#define BP_REFLECT_DEFINITIONS(args...)

class IReflectable
{
public:
    virtual const BPReflectedStruct& GetReflectionInfo() const
    {
        static BPReflectedStruct empty;
        return empty;
    }

protected:
    IReflectable();
};

#endif // IREFLECTABLE_H
