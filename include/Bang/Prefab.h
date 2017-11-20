#ifndef BANGFAB_H
#define BANGFAB_H

#include "Bang/Asset.h"
#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

class Prefab : public Asset
{
    ASSET(Prefab)

public:
    GameObject* Instantiate() const;
    GameObject* InstantiateWithoutStarting() const;

    // Resource
    void Import(const Path& prefabFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    String m_gameObjectXMLInfoContent = "";

    Prefab();
    Prefab(const Prefab &p);
    Prefab(GameObject *go);
    Prefab(const String &gameObjectXMLInfoContent);
    virtual ~Prefab();
};

NAMESPACE_BANG_END

#endif // BANGFAB_H
