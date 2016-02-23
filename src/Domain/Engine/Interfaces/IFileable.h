#ifndef IFILEABLE_H
#define IFILEABLE_H

#include "Bang.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>

//Describes something than can be written and read to/from a file
class IFileable
{
protected:
    IFileable() {}

public:
    virtual void Write(std::ostream &f) const = 0;
    virtual void Read(std::istream &f) = 0;
};

#endif // IFILEABLE_H
