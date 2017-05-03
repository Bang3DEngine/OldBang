#include "Bang/PrefabFile.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/Prefab.h"
#include "Bang/XMLParser.h"
#include "Bang/GameObject.h"
#include "Bang/AssetsManager.h"

#ifdef BANG_EDITOR
#include "Bang/Inspector.h"
#include "Bang/IconManager.h"
#include "Bang/InspectorWidget.h"
#include "Bang/PrefabFileInspectable.h"
#endif

PrefabFile::PrefabFile()
{

}

PrefabFile::PrefabFile(const Path &path) : File(path)
{
}

PrefabFile::~PrefabFile()
{
}

#ifdef BANG_EDITOR
const QPixmap& PrefabFile::GetIcon() const
{
    return IconManager::LoadPixmap(EPATH("Icons/PrefabAssetIcon.png"),
                                   IconManager::IconOverlay::Asset);
}

IInspectable *PrefabFile::GetNewInspectable()
{
    return new PrefabFileInspectable(*this);
}
#endif
