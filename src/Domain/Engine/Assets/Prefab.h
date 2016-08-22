#ifndef BANGFAB_H
#define BANGFAB_H

#include "Bang.h"



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
    const static String GetFileExtensionStatic() { return "bprefab"; }
    const virtual String GetFileExtension()
    {
        return Prefab::GetFileExtensionStatic();
    }


private:
    String m_gameObjectXMLInfoContent = "";

public:
    Prefab();
    Prefab(const Prefab &p);
    Prefab(GameObject *go);
    Prefab(const String &gameObjectXMLInfoContent);

    GameObject* Instantiate() const;
    GameObject* InstantiateWithoutStarting() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // BANGFAB_H
