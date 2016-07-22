#include "Prefab.h"

Prefab::Prefab()
{

}

Prefab::Prefab(const Prefab &p)
{
    this->m_assetDescription = p.m_assetDescription;
}

Prefab::Prefab(GameObject *o)
{
    if (o )
    {
        std::ostringstream oss;
        o->WriteInternal(oss);
        m_assetDescription = oss.str();
    }
}

Prefab::Prefab(const std::string &assetDescription)
{
    this->m_assetDescription = assetDescription;
}

GameObject *Prefab::Instantiate() const
{
    GameObject *o = InstantiateWithoutStarting();
    if (o )
    {
        o->_OnStart();
    }
    return o;
}

GameObject *Prefab::InstantiateWithoutStarting() const
{
    if (m_assetDescription != "")
    {
        std::istringstream iss (m_assetDescription);
        GameObject *o = new GameObject();
        FileReader::ReadNextLine(iss); //Consume opening tag
        o->ReadInternal(iss);
        return o;
    }
    return nullptr;
}

std::string Prefab::GetTag() const
{
    return "Prefab";
}

void Prefab::WriteInternal(std::ostream &f) const
{
    Asset::WriteInternal(f);

    GameObject *o = InstantiateWithoutStarting();
    if (o )
    {
        o->WriteInternal(f);
        delete o;
    }
}

void Prefab::ReadInternal(std::istream &f)
{
    Asset::ReadInternal(f);

    //Copy contents of the read file in assetDescription,
    //to be able to use it from Instantiate()
    std::string line;
    m_assetDescription = "";
    while ( FileReader::ReadNextLine(f, &line) )
    {
        m_assetDescription += line + "\n";
    }
    //assetDescription += line + "\n";
}
