#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "Bang/Path.h"
#include "Bang/G_Image.h"

class ImageIO
{
public:
    static void Export(const Path& filepath, const G_Image &img);
    static void Import(const Path& filepath, G_Image *img, bool *ok = nullptr);

    ImageIO() = delete;

private:
    static void ExportPNG(const Path& filepath, const G_Image &img);
    static void ImportPNG(const Path& filepath, G_Image *img, bool *ok);

    static void ExportJPG(const Path& filepath, const G_Image &img, int quality);
    static void ImportJPG(const Path& filepath, G_Image *img, bool *ok);
};

#endif // IMAGEREADER_H
