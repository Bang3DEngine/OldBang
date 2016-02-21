#ifndef ASSET_H
#define ASSET_H

#include <string>

#include "FileReader.h"

#include "IToString.h"
#include "IFileable.h"

class AssetsManager;
class Asset : public IToString, public IFileable
{
friend class AssetsManager;

protected:
    std::string filepath;

    Asset();
    virtual ~Asset();

public:

    std::string GetFilepath() const;

    virtual const std::string ToString() const override;

    virtual void Write(std::ostream &f) const override;
    virtual void Read(std::istream &f) override;
};

#endif // ASSET_H
