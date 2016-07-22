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

    virtual std::string GetTag() const = 0;
    virtual void Write(std::ostream &f) const;
    virtual void WriteInternal(std::ostream &f) const = 0; // Write without open/close tags
    virtual void Read(std::istream &f);
    virtual void ReadInternal(std::istream &f) = 0;                // Read without open/close tags
};

#endif // IFILEABLE_H
