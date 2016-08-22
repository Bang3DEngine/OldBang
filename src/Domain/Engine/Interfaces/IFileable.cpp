#include "IFileable.h"

#include "FileWriter.h"
#include "FileReader.h"

String IFileable::GetXMLInfoString() const
{
    XMLNode *xmlInfo = new XMLNode();
    FillXMLInfo(xmlInfo);
    String result = xmlInfo->ToString();
    delete xmlInfo;
    return result;
}

void IFileable::ReadXMLInfoFromString(const String &xmlInfoString)
{
    XMLNode *xmlInfo = XMLNode::FromString(xmlInfoString);
    if (xmlInfo)
    {
        ReadXMLInfo(xmlInfo);
        delete xmlInfo;
    }
}

void IFileable::ReadXMLInfo(const XMLNode *xmlInfo)
{
    XMLParser::RegisterId(xmlInfo, this);
}

void IFileable::FillXMLInfo(XMLNode *xmlInfo) const
{
    XMLParser::RegisterId(xmlInfo, this);
}
