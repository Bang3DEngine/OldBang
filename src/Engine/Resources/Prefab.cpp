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
    GameObject *go = InstantiateRaw();
    go->SetParent(SceneManager::GetActiveScene());
    return go;
}

GameObject *Prefab::InstantiateRaw() const
{
    GameObject *go = GameObjectFactory::CreateGameObject(false);

    if (!GetInfoContent().IsEmpty())
    {
        XMLNode xmlInfo = XMLNodeReader::FromString(GetInfoContent());
        go->ImportXML(xmlInfo);
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

const String &Prefab::GetInfoContent() const
{
    return m_gameObjectXMLInfoContent;
}

void Prefab::Import(const Path &prefabFilepath)
{
    ImportXMLFromFile( ImportFilesManager::GetImportFilepath(prefabFilepath) );
}

void Prefab::ImportXML(const XMLNode &xmlInfo)
{
    Asset::ImportXML(xmlInfo);

    String newXMLInfo = xmlInfo.ToString();
    if (newXMLInfo != GetInfoContent())
    {
        m_gameObjectXMLInfoContent = newXMLInfo;
    }
}

void Prefab::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);

    *xmlInfo = XMLNodeReader::FromString(GetInfoContent());
}
