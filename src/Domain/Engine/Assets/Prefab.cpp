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
    if(o )
    {
        std::ostringstream oss;
        o->Write(oss);
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
    if(o )
    {
        o->_OnStart();
    }
    return o;
}

GameObject *Prefab::InstantiateWithoutStarting() const
{
    if(m_assetDescription != "")
    {
        std::istringstream iss (m_assetDescription);
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
    if(o )
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
    m_assetDescription = "";
    while( FileReader::ReadNextLine(f, &line) )
    {
        m_assetDescription += line + "\n";
    }
    //assetDescription += line + "\n";
}
#endif
