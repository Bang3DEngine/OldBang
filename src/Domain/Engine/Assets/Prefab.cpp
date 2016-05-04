#include "Prefab.h"

Prefab::Prefab()
{

}

Prefab::Prefab(const Prefab &p)
{
    this->assetDescription = p.assetDescription;
}

Prefab::Prefab(GameObject *e)
{
    if(e != nullptr)
    {
        std::ostringstream oss;
        e->Write(oss);
        assetDescription = oss.str();
    }
}

Prefab::Prefab(const std::string &assetDescription)
{
    this->assetDescription = assetDescription;
}

GameObject *Prefab::Instantiate() const
{
    GameObject *e = InstantiateWithoutStarting();
    if(e != nullptr)
    {
        e->_OnStart();
    }
    return e;
}

GameObject *Prefab::InstantiateWithoutStarting() const
{
    if(assetDescription != "")
    {
        std::istringstream iss (assetDescription);
        GameObject *e = new GameObject();
        e->Read(iss);
        return e;
    }
    return nullptr;
}

#ifdef BANG_EDITOR
void Prefab::Write(std::ostream &f) const
{
    GameObject *e = InstantiateWithoutStarting();
    if(e != nullptr)
    {
        e->Write(f);
        delete e;
    }
}

void Prefab::Read(std::istream &f)
{
    //Copy contents of the read file in assetDescription,
    //to be able to use it from Instantiate()
    std::string line;
    assetDescription = "";
    while( FileReader::ReadNextLine(f, &line) )
    {
        assetDescription += line + "\n";
    }
    assetDescription += line + "\n";
}
#endif
