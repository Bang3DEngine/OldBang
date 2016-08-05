#include "IFileable.h"

#include "FileWriter.h"
#include "FileReader.h"

std::string IFileable::GetXMLInfoString() const
{
    XMLNode *xmlInfo = new XMLNode();
    FillXMLInfo(xmlInfo);
    std::string result = xmlInfo->ToString();
    delete xmlInfo;
    return result;
}

void IFileable::ReadXMLInfo(const XMLNode *xmlInfo)
{
    XMLParser::RegisterId(xmlInfo, this);
}

void IFileable::FillXMLInfo(XMLNode *xmlInfo) const
{
    XMLParser::RegisterId(xmlInfo, this);
}
