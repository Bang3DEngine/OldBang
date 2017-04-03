#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include "Bang/WinUndef.h"

#include "Bang/String.h"

class Image
{
public:
    enum Format { RGB, RGBA, ARGB };

    Image();

    void ConvertToRGBA8();

    const unsigned char* GetData() const;
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;
    unsigned int GetBitsPerTexel() const;
    unsigned int GetBytesPerTexel() const;
    unsigned int GetBytesCount() const;

    static Image FromFile(const String &filepath);
    static Image FromQImage(const QImage &qImage);

    Format GetFormat() const;

private:
    QImage m_image;
};

#endif // IMAGE_H
