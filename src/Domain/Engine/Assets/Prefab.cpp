#include "Prefab.h"

Prefab::Prefab()
{

}

Prefab::Prefab(const Prefab &p)
{
    this->m_assetDescription = p.m_assetDescription;
}

Prefab::Prefab(GameObject *go)
{
    if (go)
    {
        XMLNode *xmlInfo = new XMLNode();
        go->FillXMLInfo(xmlInfo);
        m_assetDescription = xmlInfo->ToString();
    }
}

Prefab::Prefab(const std::string &assetDescription)
{
    this->m_assetDescription = assetDescription;
}

GameObject *Prefab::Instantiate() const
{
    GameObject *go = InstantiateWithoutStarting();
    if (go)
    {
        go->_OnStart();
    }
    return go;
}

GameObject *Prefab::InstantiateWithoutStarting() const
{
    if (m_assetDescription != "")
    {
        GameObject *go = new GameObject();
        XMLNode *xmlInfo = XMLParser::FromString(m_assetDescription); go->FillXMLInfo(xmlInfo);
        return go;
    }
    return nullptr;
}

void Prefab::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);

    m_assetDescription = xmlInfo->ToString();
}

void Prefab::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Prefab");

    GameObject *go = InstantiateWithoutStarting();
    if (go)
    {
        go->FillXMLInfo(xmlInfo);
        delete go;
    }
}
