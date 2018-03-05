#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "Bang/Path.h"
#include "Bang/Image.h"

NAMESPACE_BANG_BEGIN

FORWARD   class Texture2D;
FORWARD_T class Image;

class ImageIO
{
public:
    static void Export(const Path& filepath, const Imageb &img);
    static void Import(const Path& filepath, Imageb *img, bool *ok = nullptr);
    static void Import(const Path& filepath, Texture2D *tex, bool *ok = nullptr);

    ImageIO() = delete;

private:
    static void ExportBMP(const Path& filepath, const Imageb &img);
    static void ImportBMP(const Path& filepath, Imageb *img, bool *ok);

    static void ExportPNG(const Path& filepath, const Imageb &img);
    static void ImportPNG(const Path& filepath, Imageb *img, bool *ok);

    static void ExportJPG(const Path& filepath, const Imageb &img, int quality);
    static void ImportJPG(const Path& filepath, Imageb *img, bool *ok);

    static void ImportDDS(const Path& filepath, Texture2D *tex, bool *ok);

    static void ExportTGA(const Path& filepath, const Imageb &img);
    static void ImportTGA(const Path& filepath, Imageb *img, bool *ok);
};

NAMESPACE_BANG_END

#endif // IMAGEREADER_H
