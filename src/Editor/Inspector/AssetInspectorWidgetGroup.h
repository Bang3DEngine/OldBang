#ifndef ASSETINSPECTORWIDGETGROUP_H
#define ASSETINSPECTORWIDGETGROUP_H

#include "Bang/AssetsManager.h"
#include "Bang/FileInspectorWidgetGroup.h"

template<class AssetClass>
class AssetInspectorWidgetGroup : public FileInspectorWidgetGroup
{
public:
    AssetInspectorWidgetGroup(FileInspectorWidget *fileInspWidget)
        : FileInspectorWidgetGroup(fileInspWidget)
    {
    }

    virtual ~AssetInspectorWidgetGroup()
    {
    }

    virtual void OnInspectorWidgetChanged(IInspectorWidget *inspWidget) override
    {
        // When something changes, rewrite the Asset to its file
        FileInspectorWidgetGroup::OnInspectorWidgetChanged(inspWidget);

        File f = p_fileInspectorWidget->GetFile();
        AssetClass *asset = AssetsManager::Load<AssetClass>(f.GetPath());
        if (asset)
        {
            asset->WriteToFile(f.GetPath());
        }
    }
};

#endif // ASSETINSPECTORWIDGETGROUP_H
