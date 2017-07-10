#ifndef ASSET_H
#define ASSET_H

#include "Bang/Paths.h"
#include "Bang/Path.h"
#include "Bang/Object.h"
#include "Bang/IToString.h"
#include "Bang/SerializableObject.h"

class AssetsManager;
class Asset : public IToString,
              public SerializableObject
{
    OBJECT(Asset)

public:
    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override;

    const Path& GetFilepath() const;

    virtual void CloneInto(ICloneable *clone) const override;

    virtual String ToString() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Path m_assetFilepath;

    Asset();
    virtual ~Asset();

    friend class AssetsManager;
};

#endif // ASSET_H
