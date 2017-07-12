#include "Bang/PrefabFile.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Prefab.h"
#include "Bang/XMLParser.h"
#include "Bang/GameObject.h"
#include "Bang/AssetsManager.h"

#ifdef BANG_EDITOR
#include "Bang/Inspector.h"
#include "Bang/IconManager.h"
#include "Bang/InspectorWidget.h"
#endif

PrefabFile::PrefabFile()
{

}

PrefabFile::PrefabFile(const Path &path) : BFile(path)
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
#endif
