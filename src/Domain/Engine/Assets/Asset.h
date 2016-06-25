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
class Asset : public IToString, public IFileable
              #ifdef BANG_EDITOR
              , public IInspectable
              #endif
{
    friend class AssetsManager;

public:
    const static std::string GetFileExtensionStatic() { return "basset"; }
    const virtual std::string GetFileExtension()
    {
        return Asset::GetFileExtensionStatic();
    }

    void OnSlotValueChanged(InspectorWidget *source) {}

protected:
    std::string filepath = "";

    Asset();
    virtual ~Asset();

public:

    std::string GetFilepath() const;

    virtual const std::string ToString() const override;

    virtual void Write(std::ostream &f) const override;
    virtual void Read(std::istream &f) override;
};

#endif // ASSET_H
