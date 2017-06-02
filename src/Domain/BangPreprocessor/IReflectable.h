#ifndef IREFLECTABLE_H
#define IREFLECTABLE_H

#include "Bang/BPReflectedStruct.h"

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
