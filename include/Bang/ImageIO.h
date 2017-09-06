#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "Bang/Path.h"
#include "Bang/Image.h"

class ImageIO
{
public:
    static void Export(const Path& filepath, const Imageb &img);
    static void Import(const Path& filepath, Imageb *img, bool *ok = nullptr);

    ImageIO() = delete;

private:
    static void ExportPNG(const Path& filepath, const Imageb &img);
    static void ImportPNG(const Path& filepath, Imageb *img, bool *ok);

    static void ExportJPG(const Path& filepath, const Imageb &img, int quality);
    static void ImportJPG(const Path& filepath, Imageb *img, bool *ok);
};

#endif // IMAGEREADER_H
