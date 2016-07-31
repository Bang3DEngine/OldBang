#include "IFileable.h"

#include "FileWriter.h"
#include "FileReader.h"


void IFileable::ReadXMLInfo(const XMLNode *xmlInfo)
{
    XMLParser::RegisterId(xmlInfo, this);
}

void IFileable::FillXMLInfo(XMLNode *xmlInfo) const
{
    XMLParser::RegisterId(xmlInfo, this);
}
