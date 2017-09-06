#ifndef ITOSTRING_H
#define ITOSTRING_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class IToString
{
protected:
    IToString() {}

public:
    virtual String ToString() const = 0;
};

NAMESPACE_BANG_END

#endif // ITOSTRING_H
