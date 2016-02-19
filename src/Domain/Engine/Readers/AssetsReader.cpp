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
            Logger_Log(line);
            if(line == "<Material>") a = ReadAsset<Material>(f);
            else if(line == "<Mesh>") a = ReadAsset<Mesh>(f);
            else if(line == "<Texture2D>") a = ReadAsset<Texture2D>(f);
            Logger_Log("_");
        }
    }
}
