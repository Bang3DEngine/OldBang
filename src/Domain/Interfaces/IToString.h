#ifndef ITOSTRING_H
#define ITOSTRING_H

#include <iostream>
#include <sstream>
#include <string>

class IToString
{
protected:
    IToString() {}

public:
    virtual const std::string ToString() const = 0;
};

#endif // ITOSTRING_H
