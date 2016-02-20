#include "AssetsReader.h"

#include "Texture2D.h"
#include "Material.h"
#include "Mesh.h"

void AssetsReader::ReadAssetsFile(const std::string &filepath)
{
    std::ifstream f (filepath);
    if ( !f.is_open() )
    {
        Logger_Error("Could not open the file '" << filepath << "' to load the assets.");
    }
    else
    {
        L("Skip <assets>..."); FileReader::ReadNextLine(f); //skip <assets> line

        std::string line;
        while( (line = FileReader::ReadNextLine(f)) != "</assets>" )
        {
            Asset *a = nullptr;
            L("assetFilepath...");
            std::string assetFilepath = FileReader::ReadString(f);

            if(line == "<Material>") a = ReadAssetFile<Material>(assetFilepath);
            else if(line == "<Mesh>") a = ReadAssetFile<Mesh>(assetFilepath);
            else if(line == "<Texture2D>") a = ReadAssetFile<Texture2D>(assetFilepath);

            L("Consume closing tag..."); FileReader::ReadNextLine(f); //Consume closing tag
            if(a != nullptr)
            {
                Logger_Log("Saving asset..."); AssetsManager::SaveAsset(a->GetFilepath(), a);
            }
        }
    }
}
