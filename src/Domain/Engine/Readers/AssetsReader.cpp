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
        FileReader::ReadNextLine(f); //skip <assets> line

        std::string line;
        while( (line = FileReader::ReadNextLine(f)) != "</assets>" )
        {
            Asset *a = nullptr;
            std::string assetFilepath = FileReader::ReadString(f);
            if(line == "<Material>") a = ReadAssetFile<Material>(assetFilepath);
            else if(line == "<Mesh>") a = ReadAssetFile<Mesh>(assetFilepath);
            else if(line == "<Texture2D>") a = ReadAssetFile<Texture2D>(assetFilepath);


            FileReader::ReadNextLine(f); //Consume closing tag
            AssetsManager::SaveAsset(a->GetFilepath(), a);
        }
    }
}
