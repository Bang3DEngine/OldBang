#include "Prefab.h"

Prefab::Prefab()
{

}

Prefab::Prefab(const Prefab &p)
{
    this->assetDescription = p.assetDescription;
}

Prefab::Prefab(GameObject *o)
{
    if(o != nullptr)
    {
        std::ostringstream oss;
        o->Write(oss);
        assetDescription = oss.str();
    }
}

Prefab::Prefab(const std::string &assetDescription)
{
    this->assetDescription = assetDescription;
}

GameObject *Prefab::Instantiate() const
{
    GameObject *o = InstantiateWithoutStarting();
    if(o != nullptr)
    {
        o->_OnStart();
    }
    return o;
}

GameObject *Prefab::InstantiateWithoutStarting() const
{
    if(assetDescription != "")
    {
        std::istringstream iss (assetDescription);
        GameObject *o = new GameObject();
        FileReader::ReadNextLine(iss); //Consume opening tag
        o->Read(iss);
        return o;
    }
    return nullptr;
}

#ifdef BANG_EDITOR
void Prefab::Write(std::ostream &f) const
{
    GameObject *o = InstantiateWithoutStarting();
    if(o != nullptr)
    {
        o->Write(f);
        delete o;
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
