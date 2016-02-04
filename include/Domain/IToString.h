#ifndef ITOSTRING_H
#define ITOSTRING_H

#include <string>

class IToString
{
protected:
    IToString() {}

public:
    virtual const std::string ToString() const = 0;
};

#endif // ITOSTRING_H
