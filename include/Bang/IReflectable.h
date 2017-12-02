#ifndef IREFLECTABLE_H
#define IREFLECTABLE_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

#define BP_REFLECT_VARIABLE(...)
#define BP_REFLECT_CLASS(...)
#define BP_REFLECT_STRUCT(...)
#define BP_REFLECT_DEFINITIONS(...)

FORWARD class BPReflectedStruct;

class IReflectable
{
public:
    virtual const BPReflectedStruct& GetReflectionInfo() const;

protected:
    IReflectable() {}
    virtual ~IReflectable() {}
};

NAMESPACE_BANG_END

#endif // IREFLECTABLE_H
