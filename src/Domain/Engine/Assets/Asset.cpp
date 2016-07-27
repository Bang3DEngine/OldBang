#include "Asset.h"

Asset::Asset()
{

}

Asset::~Asset()
{

}

std::string Asset::GetFilepath() const
{
    return m_filepath;
}

const std::string Asset::ToString() const
{
    return "Asset '" + m_filepath + "'.";
}


void Asset::ReadXMLNode(const XMLNode *xmlNode)
{
}

void Asset::GetXMLNode(XMLNode *xmlNode) const
{
    xmlNode->SetTagName("Asset");
    xmlNode->AddAttribute("filepath", m_filepath);
}
