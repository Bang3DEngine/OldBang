#include "InspectorTexture2DAssetWidget.h"

#include "Texture2D.h"
#include "Persistence.h"
#include "AssetsManager.h"

InspectorTexture2DAssetWidget::InspectorTexture2DAssetWidget
    (const FileTexture2DAsset &fileTex) : InspectorWidget()
{
    assetFilepath = fileTex.GetPath();
    assetFilepath = Persistence::ProjectRootAbsoluteToRelative(assetFilepath);

    //Temp tex to get the needed values
    Texture2D *tex =
            AssetsManager::ReadTmpAsset<Texture2D>(fileTex.GetPath());
    std::string imgFilepath = tex->GetImageRelativeFilepath();

    inspectorInfo.SetSlotsInfos(
        {
            new InspectorStringSWInfo("Image path", imgFilepath, true),
            new InspectorEnumSWInfo("Filter", {"Nearest", "Linear"},
                  tex->GetFilterMode() == Texture2D::FilterMode::Nearest ?
                    0 : 1)
        }
    );

    delete tex;

    this->ConstructFromWidgetInformation(fileTex.GetName(), &inspectorInfo,
                                         false);
}

InspectorTexture2DAssetWidget::~InspectorTexture2DAssetWidget()
{

}

void InspectorTexture2DAssetWidget::_OnSlotValueChanged()
{
    //First we have to update the instance in the AssetsManager, (if any)
    //To enable a live change ingame of the Asset being used by Parts/Entities
    Texture2D *currentTex = AssetsManager::
            GetCachedAsset<Texture2D>(assetFilepath);

    bool tmpAssetLoaded = false;
    if(currentTex == nullptr)
    {
        //Load a tmp asset, to be able to save it to a file.
        tmpAssetLoaded = true;
        currentTex = AssetsManager::ReadTmpAsset<Texture2D>(assetFilepath);
    }

    if(currentTex != nullptr)
    {
        //Only update the current Asset instance being used if it exists
        int filterModeIndex = this->GetSWSelectedEnumIndex("Filter");
        Texture2D::FilterMode fm = filterModeIndex == 0 ?
                    Texture2D::FilterMode::Nearest :
                    Texture2D::FilterMode::Linear;
        currentTex->SetFilterMode(fm);

        //Save the modified asset
        FileWriter::WriteAsset(assetFilepath, currentTex);
        if(tmpAssetLoaded)
        {
            delete currentTex;
        }
    }
}
