#include "Bang/PrefabFile.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/Prefab.h"
#include "Bang/Inspector.h"
#include "Bang/XMLParser.h"
#include "Bang/GameObject.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/InspectorWidget.h"
#include "Bang/PrefabAssetFileInspectable.h"

PrefabFile::PrefabFile()
{

}

PrefabFile::PrefabFile(const QFileSystemModel *model, const QModelIndex &index)
    : PrefabFile( model->filePath(index) )
{
}

PrefabFile::PrefabFile(const String &absPath) : File(absPath)
{
    m_tmpGameObject = new GameObject();
    m_tmpGameObject->ReadFromFile( absPath );
}

PrefabFile::~PrefabFile()
{
    delete m_tmpGameObject;
}

const QPixmap& PrefabFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/PrefabAssetIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Asset);
}

void PrefabFile::Read(const XMLNode &xmlInfo)
{
    m_tmpGameObject->Read(xmlInfo);
}

void PrefabFile::Write(XMLNode *xmlInfo) const
{
    m_tmpGameObject->Write(xmlInfo);
}

#ifdef BANG_EDITOR
void PrefabFile::OnInspectorSlotChanged(InspectorWidget *inspectorItem)
{
    AssetsManager::UpdateAsset(GetAbsolutePath(),
                               m_tmpGameObject->GetXMLInfo());
}

IInspectable *PrefabFile::GetNewInspectable() const
{
    return nullptr;//new PrefabAssetFileInspectable(*this);
}
#endif
