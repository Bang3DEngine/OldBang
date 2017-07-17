#ifndef ASSETFILECREATOR_H
#define ASSETFILECREATOR_H

#include "Bang/Path.h"
#include "Bang/Explorer.h"
#include "Bang/Extensions.h"
#include "Bang/AssetsManager.h"

class AssetFileCreator
{
public:
    template<class AssetType>
    static Path Create(const Path &_assetNewPath)
    {
        Path assetNewPath = _assetNewPath
                             .AppendExtension(Extensions::Get<AssetType>())
                             .GetDuplicate();

        AssetsManager::Create<AssetType>(assetNewPath);
        return assetNewPath;
    }

    template<class AssetType>
    static Path CreateOnExplorer(const String &_assetNewName = "")
    {
        String assetNewName = _assetNewName;
        if (assetNewName.Empty())
        {
            assetNewName = "New_" + AssetType::GetClassNameStatic();
        }

        Path filepath =
           Explorer::GetInstance()->GetCurrentDir()
                           .Append(assetNewName)
                           .AppendExtension(Extensions::Get<AssetType>());

        filepath = AssetFileCreator::Create<AssetType>(filepath);
        Explorer::GetInstance()->StartRenaming(filepath);

        return filepath;
    }

private:
    AssetFileCreator();
};

#endif // ASSETFILECREATOR_H
