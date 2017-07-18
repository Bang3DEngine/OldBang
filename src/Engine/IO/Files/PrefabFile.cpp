#include "Bang/PrefabFile.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Prefab.h"
#include "Bang/XMLParser.h"
#include "Bang/GameObject.h"
#include "Bang/AssetsManager.h"

PrefabFile::PrefabFile()
{

}

PrefabFile::PrefabFile(const Path &path) : BFile(path)
{
}

PrefabFile::~PrefabFile()
{
}
