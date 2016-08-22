#ifndef ITOSTRING_H
#define ITOSTRING_H

#include "Bang.h"

#include <iostream>
#include <sstream>


class IToString
{
protected:
    IToString() {}

public:
    virtual const String ToString() const = 0;
};

#endif // ITOSTRING_H
