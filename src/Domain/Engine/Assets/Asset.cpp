#include "Asset.h"

Asset::Asset()
{

}

Asset::~Asset()
{

}

void Asset::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}

void Asset::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

std::string Asset::GetFilepath() const
{
    return m_filepath;
}

const std::string Asset::ToString() const
{
    return "Asset '" + m_filepath + "'.";
}


void Asset::ReadXMLInfo(const XMLNode *xmlInfo)
{
}

void Asset::FillXMLInfo(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("Asset");
    //xmlInfo->SetFilepath("filepath", m_filepath);
}
