#ifndef ITOSTRING_H
#define ITOSTRING_H

#include "Bang.h"

FORWARD class String;

class IToString
{
protected:
    IToString() {}

public:
    virtual String ToString() const = 0;
};

#endif // ITOSTRING_H
