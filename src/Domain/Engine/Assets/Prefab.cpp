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
        XMLNode *xmlNode = new XMLNode();
        go->GetXMLNode(xmlNode);
        m_assetDescription = xmlNode->ToString();
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
        XMLNode *xmlNode = XMLParser::FromXML(m_assetDescription); go->GetXMLNode(xmlNode);
        return go;
    }
    return nullptr;
}

void Prefab::ReadXMLNode(const XMLNode *xmlNode)
{
    Asset::ReadXMLNode(xmlNode);

    m_assetDescription = xmlNode->ToString();
}

void Prefab::GetXMLNode(XMLNode *xmlNode) const
{
    Asset::GetXMLNode(xmlNode);
    xmlNode->SetTagName("Prefab");

    GameObject *go = InstantiateWithoutStarting();
    if (go)
    {
        go->GetXMLNode(xmlNode);
        delete go;
    }
}
