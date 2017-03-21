#ifndef ASSET_H
#define ASSET_H

#include "IO.h"
#include "IToString.h"
#include "IconManager.h"
#include "SerializableObject.h"

#ifdef BANG_EDITOR
#include "SerializableObject.h"
#endif

class AssetsManager;
class Asset : public IToString,
              public SerializableObject
{
public:
    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override;

    String GetFilepath() const;

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
