#ifndef ITOSTRING_H
#define ITOSTRING_H

#include "Bang.h"

class IToString
{
protected:
    IToString() {}

public:
    virtual String ToString() const = 0;
};

#endif // ITOSTRING_H
