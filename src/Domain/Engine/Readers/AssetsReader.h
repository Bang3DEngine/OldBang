#ifndef ASSETSREADER_H
#define ASSETSREADER_H

#include "ReaderWithPointerIds.h"
#include "AssetsManager.h"
#include "Texture2D.h"
#include "Material.h"
#include "Asset.h"
#include "Mesh.h"

class AssetsReader : public ReaderWithPointerIds
{
private:
    AssetsReader() {}

public:
    static void ReadAssets(const std::string &filepath);
};

#endif // ASSETSREADER_H
