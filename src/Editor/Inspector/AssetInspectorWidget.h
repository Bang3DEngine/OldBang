#ifndef ASSETINSPECTORWIDGET_H
#define ASSETINSPECTORWIDGET_H

#include "Bang/AssetsManager.h"
#include "Bang/FileInspectorWidget.h"

class Asset;
template<class AssetClass>
class AssetInspectorWidget : public FileInspectorWidget
{
public:
    AssetInspectorWidget(const BFile &file) : FileInspectorWidget(file)
    {
    }

protected:
    virtual void InitExtra() override
    {
        FileInspectorWidget::InitExtra();
    }

    virtual void OnUpdate() override
    {
        FileInspectorWidget::OnUpdate();
    }

    AssetClass *GetAsset() const
    {
        return AssetsManager::Load<AssetClass>( m_file.GetPath() );
    }
};


#endif // ASSETINSPECTORWIDGET_H
