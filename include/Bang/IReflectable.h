#ifndef IREFLECTABLE_H
#define IREFLECTABLE_H

#include "Bang/Bang.h"

#define BP_REFLECT_VARIABLE(args...)
#define BP_REFLECT_CLASS(args...)
#define BP_REFLECT_STRUCT(args...)
#define BP_REFLECT_DEFINITIONS(args...)

FORWARD class BPReflectedStruct;

class IReflectable
{
public:
    virtual const BPReflectedStruct& GetReflectionInfo() const;

protected:
    IReflectable() {}
    virtual ~IReflectable() {}
};

#endif // IREFLECTABLE_H
