#include "Bang/Prefab.h"

#include "Bang/Scene.h"
#include "Bang/Resources.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/XMLNodeReader.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

Prefab::Prefab()
{

}

Prefab::Prefab(GameObject *go)
{
    SetGameObject(go);
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
    GameObject *go = GameObjectFactory::CreateGameObject(false);

    if (!m_gameObjectXMLInfoContent.IsEmpty())
    {
        XMLNode xmlInfo = XMLNodeReader::FromString(m_gameObjectXMLInfoContent);
        go->ImportXML(xmlInfo);
        go->SetEnabled(true);
    }

    return go;
}

void Prefab::SetGameObject(GameObject *go)
{
    if (go)
    {
        m_gameObjectXMLInfoContent = go->GetSerializedString();
    }
    else { m_gameObjectXMLInfoContent = ""; }
}

void Prefab::Import(const Path &prefabFilepath)
{
    ImportXMLFromFile(prefabFilepath);
}

void Prefab::ImportXML(const XMLNode &xmlInfo)
{
    Asset::ImportXML(xmlInfo);

    String newXMLInfo = xmlInfo.ToString();
    if (m_gameObjectXMLInfoContent != newXMLInfo)
    {
        m_gameObjectXMLInfoContent = xmlInfo.ToString();
    }
}

void Prefab::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);

    *xmlInfo = XMLNodeReader::FromString(m_gameObjectXMLInfoContent);
}
