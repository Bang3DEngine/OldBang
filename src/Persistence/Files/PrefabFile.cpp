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

PrefabFile::PrefabFile(const Path &path) : File(path)
{
}

PrefabFile::~PrefabFile()
{
}

const QPixmap& PrefabFile::GetIcon() const
{
    return IconManager::LoadPixmap(EPATH("Icons/PrefabAssetIcon.png"),
                                   IconManager::IconOverlay::Asset);
}

#ifdef BANG_EDITOR
IInspectable *PrefabFile::GetNewInspectable()
{
    return new PrefabAssetFileInspectable(*this);
}
#endif
