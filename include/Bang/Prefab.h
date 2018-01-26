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

    void SetGameObject(GameObject *go);

    // Resource
    void Import(const Path& prefabFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    String m_gameObjectXMLInfoContent = "";
private:

    Prefab();
    Prefab(GameObject *go);
    Prefab(const String &gameObjectXMLInfoContent);
    virtual ~Prefab();
};

NAMESPACE_BANG_END

#endif // BANGFAB_H
