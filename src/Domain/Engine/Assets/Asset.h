#ifndef ASSET_H
#define ASSET_H

#include "Bang.h"

#include <string>

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
    friend class AssetsManager;

public:
    const static std::string GetFileExtensionStatic() { return "basset"; }
    const virtual std::string GetFileExtension()
    {
        return Asset::GetFileExtensionStatic();
    }

#ifdef BANG_EDITOR
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
#endif

protected:
    std::string m_filepath = "";

    Asset();
    virtual ~Asset();

public:

    std::string GetFilepath() const;

    virtual const std::string ToString() const override;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // ASSET_H
