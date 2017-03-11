#ifndef ASSET_H
#define ASSET_H

#include "IToString.h"
#include "SerializableObject.h"

#ifdef BANG_EDITOR
#include "IInspectable.h"
#endif

class AssetsManager;
class Asset : public IToString
             ,public SerializableObject
             #ifdef BANG_EDITOR
             ,public IInspectable
             #endif
{
public:
    static String GetFileExtensionStatic();
    virtual String GetFileExtension();

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    #endif

    String GetFilepath() const;

    virtual String ToString() const override;

    virtual void Read(const XMLNode *xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    String m_assetFilepath = "";

    Asset();
    virtual ~Asset();

    friend class AssetsManager;
};

#endif // ASSET_H
