#ifndef IFILEABLE_H
#define IFILEABLE_H

#include "Bang.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>

#include "XMLParser.h"

//Describes something than can be written and read to/from a file
class IFileable
{
protected:
    IFileable() {}

public:
    virtual void ReadXMLInfo(const XMLNode *xmlInfo) = 0;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const = 0;
};

#endif // IFILEABLE_H
