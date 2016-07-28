#ifndef BANGFAB_H
#define BANGFAB_H

#include "Bang.h"

#include <string>

#include "GameObject.h"
#include "Asset.h"

//Represents an GameObject that can be saved as an asset, and
//can be instantiated.
//In fact its asset file will be of the form
//<GameObject>
//  ...
//</GameObject>
class Prefab : public Asset
{

public:
    const static std::string GetFileExtensionStatic() { return "bprefab"; }
    const virtual std::string GetFileExtension()
    {
        return Prefab::GetFileExtensionStatic();
    }


private:
    //This variable contains the description inside the
    //corresponding *.bprefab file (including the opening/closing tags)
    std::string m_assetDescription = "";

public:
    Prefab();
    Prefab(const Prefab &p);
    Prefab(GameObject *o);
    Prefab(const std::string &m_assetDescription);

    GameObject* Instantiate() const;
    GameObject* InstantiateWithoutStarting() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // BANGFAB_H
