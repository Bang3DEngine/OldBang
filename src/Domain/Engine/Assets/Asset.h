#ifndef ASSET_H
#define ASSET_H

#include "Bang/IO.h"
#include "Bang/Object.h"
#include "Bang/IToString.h"
#include "Bang/IconManager.h"
#include "Bang/SerializableObject.h"

#ifdef BANG_EDITOR
#include "Bang/SerializableObject.h"
#endif

class AssetsManager;
class Asset : public IToString,
              public SerializableObject
{
    OBJECT(Asset)

public:
    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override;

    String GetFilepath() const;

    virtual void CloneInto(ICloneable *clone) const override;

    virtual String ToString() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    String m_assetFilepath = "";

    Asset();
    virtual ~Asset();

    friend class AssetsManager;
};

#define ASSET_ICON(CLASS, ICON_PATH) \
public: \
    static const QPixmap& GetIconStatic() \
    {\
        String path = IO::ToAbsolute(ICON_PATH, true); \
        return IconManager::LoadPixmap(path); \
    } \
    const virtual QPixmap& GetIcon() const override \
    { \
        return CLASS::GetIconStatic(); \
    }

#endif // ASSET_H
