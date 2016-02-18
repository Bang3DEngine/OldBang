#ifndef ASSETSREADER_H
#define ASSETSREADER_H

#include "ReaderWithPointerIds.h"
#include "AssetsManager.h"
#include "Asset.h"
#include "Mesh.h"
#include "Material.h"

class AssetsReader : public ReaderWithPointerIds
{
private:
    AssetsReader() {}

public:
    static void ReadAssets(const std::string &filepath);
};

#endif // ASSETSREADER_H
