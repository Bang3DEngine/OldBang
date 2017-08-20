#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "Bang/Path.h"
#include "Bang/G_Image.h"

class ImageIO
{
public:
    static void Write(const Path& filepath, const G_Image &img);
    static void Read(const Path& filepath, G_Image *img, bool *ok = nullptr);

    ImageIO() = delete;

private:
    static void WritePNG(const Path& filepath, const G_Image &img);
    static void ReadPNG(const Path& filepath, G_Image *img, bool *ok);
};

#endif // IMAGEREADER_H
