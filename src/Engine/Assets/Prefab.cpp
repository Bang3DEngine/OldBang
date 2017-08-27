#include "Bang/Prefab.h"

#include "Bang/Scene.h"
#include "Bang/XMLParser.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"

Prefab::Prefab()
{

}

Prefab::Prefab(const Prefab &p)
{
    m_gameObjectXMLInfoContent = p.m_gameObjectXMLInfoContent;
}

Prefab::Prefab(GameObject *go)
{
    if (go)
    {
        m_gameObjectXMLInfoContent = go->GetSerializedString();
    }
}

Prefab::Prefab(const String &gameObjectXMLInfoContent)
{
    m_gameObjectXMLInfoContent = gameObjectXMLInfoContent;
}

Prefab::~Prefab()
{
}

GameObject *Prefab::Instantiate() const
{
    GameObject *go = InstantiateWithoutStarting();
    if (go)
    {
        Scene *scene = SceneManager::GetActiveScene();
        go->SetParent(scene);
        go->Start();
    }
    return go;
}

GameObject *Prefab::InstantiateWithoutStarting() const
{
    if (!m_gameObjectXMLInfoContent.IsEmpty())
    {
        XMLNode xmlInfo = XMLParser::FromString(m_gameObjectXMLInfoContent);

        GameObject *go = new GameObject();
        go->Read(xmlInfo);
        go->SetEnabled(true);
        return go;
    }
    return nullptr;
}

void Prefab::Import(const Path &prefabFilepath)
{
    ReadFromFile(prefabFilepath);
}

void Prefab::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);
    String newXMLInfo = xmlInfo.ToString();
    if (m_gameObjectXMLInfoContent != newXMLInfo)
    {
        m_gameObjectXMLInfoContent = xmlInfo.ToString();
    }
}

void Prefab::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    XMLNode goInfo = XMLParser::FromString(m_gameObjectXMLInfoContent);
    goInfo.CloneInto(xmlInfo);
}
