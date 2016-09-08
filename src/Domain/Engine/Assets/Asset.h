#ifndef ASSET_H
#define ASSET_H

#include "Bang.h"



#include "FileWriter.h"
#include "FileReader.h"

#include "IToString.h"
#include "IFileable.h"

#ifdef BANG_EDITOR
#include "IInspectable.h"
#endif

class AssetsManager;
class Asset : public IToString
             ,public IFileable
             #ifdef BANG_EDITOR
             ,public IInspectable
             #endif
{
public:
    const static String GetFileExtensionStatic() { return "basset"; }
    const virtual String GetFileExtension()
    {
        return Asset::GetFileExtensionStatic();
    }

#ifdef BANG_EDITOR
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
#endif

protected:
    String m_filepath = "";

    Asset();
    virtual ~Asset();

public:

    String GetFilepath() const;

    virtual const String ToString() const override;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;


    friend class AssetsManager;
};

#endif // ASSET_H
