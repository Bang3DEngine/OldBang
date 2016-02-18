#include "AssetsReader.h"

void AssetsReader::ReadAssets(const std::string &filepath)
{
    ClearPointerIds();

    std::ifstream f (filepath);
    if ( !f.is_open() )
    {
        Logger_Error("Could not open the file '" << filepath << "' to load the assets.");
    }
    else
    {
        std::string line;
        FileReader::ReadNextLine(f); //skip <assets> line
        while( (line = FileReader::ReadNextLine(f)) != "</assets>" )
        {
            Asset *a = nullptr;
            if(line == "<Material>")
            {
                Material *m = new Material();
                m->Read(f);
                a = m;
            }
            else if(line == "<Mesh>")
            {
                Mesh *m = new Mesh();
                m->Read(f);
                a = m;
            }
            else if(line == "<Texture2D>")
            {
                Texture2D *tex = new Texture2D();
                tex->Read(f);
                a = tex;
            }

            if(a != nullptr && a->GetFilepath() != "")
            {
                AssetsManager::SaveAsset(a->GetFilepath(), a);
            }
        }
    }
}
