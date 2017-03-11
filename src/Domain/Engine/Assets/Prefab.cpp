#include "Prefab.h"

#include "Scene.h"
#include "Debug.h"
#include "XMLParser.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "EditorState.h"
#endif

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

String Prefab::GetFileExtensionStatic()
{
    return "bprefab";
}

String Prefab::GetFileExtension()
{
    return Prefab::GetFileExtensionStatic();
}

GameObject *Prefab::Instantiate() const
{
    GameObject *go = InstantiateWithoutStarting();
    bool canStart = go;

    #ifdef BANG_EDITOR
    canStart = canStart && EditorState::IsPlaying();
    #endif

    if (canStart)
    {
        Scene *scene = SceneManager::GetActiveScene();
        go->SetParent(scene);
        go->_OnStart();
    }
    return go;
}

GameObject *Prefab::InstantiateWithoutStarting() const
{
    if (!m_gameObjectXMLInfoContent.Empty())
    {
        XMLNode *xmlInfo = XMLParser::FromString(m_gameObjectXMLInfoContent);
        GameObject *go = new GameObject();
        go->Read(xmlInfo);
        return go;
    }
    return nullptr;
}

void Prefab::Read(const XMLNode *xmlInfo)
{
    Asset::Read(xmlInfo);
    String newXMLInfo = xmlInfo->ToString();
    if (m_gameObjectXMLInfoContent != newXMLInfo)
    {
        m_gameObjectXMLInfoContent = xmlInfo->ToString();
        #ifdef BANG_EDITOR
        AssetsManager::OnAssetFileChanged<Prefab>(m_assetFilepath, xmlInfo);
        #endif
    }
}

void Prefab::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    XMLNode *goInfo = XMLParser::FromString(m_gameObjectXMLInfoContent);
    if (goInfo)
    {
        goInfo->CloneInto(xmlInfo);
        xmlInfo->SetTagName("Prefab");
        delete goInfo;
    }
}
