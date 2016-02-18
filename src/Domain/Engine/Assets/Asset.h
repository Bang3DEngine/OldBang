#ifndef ASSET_H
#define ASSET_H

#include <string>

#include "IToString.h"
#include "IFileable.h"

class Asset : public IToString, public IFileable
{
protected:
    std::string filepath;

    Asset();
    virtual ~Asset();

public:

    std::string GetFilepath() const;

    virtual const std::string ToString() const override;
};

#endif // ASSET_H
