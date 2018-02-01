#ifndef ASSET_H
#define ASSET_H

#include "Bang/Path.h"
#include "Bang/Object.h"
#include "Bang/Resource.h"
#include "Bang/IToString.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

#define ASSET(CLASSNAME) \
    RESOURCE(CLASSNAME) \
    friend class Asset;

class Asset : public Resource,
              public IToString,
              public IEnabledListener,
              public EventEmitter<IDestroyListener>,
              public EventEmitter<IEnabledListener>
{
public:
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
    static AssetClass* Create(Args... args) { return new AssetClass(args...); }

    static void Destroy(Asset *asset);

    friend class Resources;
};

NAMESPACE_BANG_END

#endif // ASSET_H
