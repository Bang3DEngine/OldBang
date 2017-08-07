#ifndef ASSET_H
#define ASSET_H

#include "Bang/Path.h"
#include "Bang/IToString.h"
#include "Bang/SerializableObject.h"

class Asset : public IToString,
              public SerializableObject
{
    SOBJECT(Asset)

public:
    const Path& GetFilepath() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // IToString
    virtual String ToString() const override;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Path m_assetFilepath;

    Asset();
    virtual ~Asset();

    friend class AssetsManager;
};

#endif // ASSET_H
