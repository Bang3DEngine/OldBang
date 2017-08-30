#ifndef ASSET_H
#define ASSET_H

#include "Bang/Path.h"
#include "Bang/Resource.h"
#include "Bang/IToString.h"

#define ASSET(CLASS_NAME) RESOURCE(CLASS_NAME)

class Asset : public Resource,
              public IToString
{
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

    friend class Resources;
};

#endif // ASSET_H