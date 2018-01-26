#ifndef ASSET_H
#define ASSET_H

#include "Bang/Path.h"
#include "Bang/Object.h"
#include "Bang/Resource.h"
#include "Bang/IToString.h"
#include "Bang/ObjectManager.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

#define ASSET(CLASSNAME) \
    RESOURCE(CLASSNAME) \
    friend class Asset; \
    friend class ObjectManager;

class Asset : public Resource,
              public IToString,
              public Object
{
    RESOURCE_NO_CLONEABLE(CLASSNAME)
    friend class ObjectManager;

public:
    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // IToString
    virtual String ToString() const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    Asset();
    virtual ~Asset();

private:
    template <class AssetClass, class... Args>
    static AssetClass* Create(Args... args)
    { return ObjectManager::Create<AssetClass>(args...); }

    static void Destroy(Asset *asset);
};

NAMESPACE_BANG_END

#endif // ASSET_H
