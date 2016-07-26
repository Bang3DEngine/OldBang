#include "InspectorTexture2DAssetWidget.h"

#include "Texture2D.h"
#include "Persistence.h"
#include "AssetsManager.h"

InspectorTexture2DAssetWidget::InspectorTexture2DAssetWidget
    (const FileTexture2DAsset &fileTex) : InspectorWidget()
{
    m_assetFilepath = fileTex.GetPath();
    m_assetFilepath = Persistence::ProjectRootAbsoluteToRelative(m_assetFilepath);

    //Temp tex to get the needed values
    Texture2D *tex =
            AssetsManager::ReadTmpAsset<Texture2D>(fileTex.GetPath());
    std::string imgFilepath = tex->GetImageRelativeFilepath();

    m_inspectorInfo.AddSlotInfos(
        {
            new InspectorStringSWInfo("Image path", true),
            new InspectorEnumSWInfo("Filter", {"Nearest", "Linear"})
        }
    );

    m_inspectorInfo.GetStringSWSlotInfo("Image path")->m_value = imgFilepath;
    m_inspectorInfo.GetEnumSWSlotInfo("Filter")->m_selectedValueIndex =
            tex->GetFilterMode() == Texture2D::FilterMode::Nearest ? 0 : 1;

    delete tex;

    this->ConstructFromWidgetInformation(fileTex.GetName(), &m_inspectorInfo,
                                         false);
}

InspectorTexture2DAssetWidget::~InspectorTexture2DAssetWidget()
{

}

void InspectorTexture2DAssetWidget::_OnSlotValueChanged()
{
    //First we have to update the instance in the AssetsManager, (if any)
    //To enable a live change ingame of the Asset being used by Components/Entities
    Texture2D *currentTex = AssetsManager::
            GetCachedAsset<Texture2D>(m_assetFilepath);


    bool tmpAssetLoaded = false;
    if (!currentTex)
    {
        //Load a tmp asset, to be able to save it to a file.
        tmpAssetLoaded = true;
        currentTex = AssetsManager::ReadTmpAsset<Texture2D>(m_assetFilepath);
    }

    if (currentTex )
    {
        //Only update the current Asset instance being used if it exists
        int filterModeIndex = this->GetSWSelectedEnumIndex("Filter");
        Texture2D::FilterMode fm = filterModeIndex == 0 ?
                    Texture2D::FilterMode::Nearest :
                    Texture2D::FilterMode::Linear;
        currentTex->SetFilterMode(fm);

        //Save the modified asset
        FileWriter::WriteAsset(m_assetFilepath, currentTex);
        if (tmpAssetLoaded)
        {
            delete currentTex;
        }
    }
}
