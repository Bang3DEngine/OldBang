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
    void OnSlotValueChanged(InspectorWidget *source) {}
#endif

protected:
    std::string m_filepath = "";

    Asset();
    virtual ~Asset();

public:

    std::string GetFilepath() const;

    virtual const std::string ToString() const override;

    virtual std::string GetTag() const override;
    virtual void WriteInternal(std::ostream &f) const override;
    virtual void ReadInternal(std::istream &f) override;
};

#endif // ASSET_H
