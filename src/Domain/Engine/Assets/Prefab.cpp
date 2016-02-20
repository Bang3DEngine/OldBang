#include "Prefab.h"

Prefab::Prefab()
{

}

Prefab::Prefab(const std::string &assetDescription)
{
    this->assetDescription = assetDescription;
}

Entity *Prefab::Instantiate() const
{
    if(assetDescription != "")
    {
        std::istringstream iss (assetDescription);
        Entity *e = new Entity();
        e->Read(iss);
        return e;
    }

    return nullptr;
}

void Prefab::Write(std::ostream &f) const
{

}

void Prefab::Read(std::istream &f)
{
    //Copy contents of the read file in assetDescription,
    // to be able to use it from Instantiate()
    assetDescription = "";
    std::string line;
    while( (line = FileReader::ReadNextLine(f)) != "</Entity>")
    {
        assetDescription += line + "\n";
    }
    assetDescription += line + "\n"; // copy last line "</Entity>"
}
