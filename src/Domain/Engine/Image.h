#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include "Bang/WinUndef.h"

#include "Bang/Array.h"
#include "Bang/String.h"

class Image
{
public:
    Image();

    void SetPixel(int x, int y, const Color& color);

    const unsigned char* GetData8() const;
    const Color& GetPixel(int x, int y) const;
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    static Image FromFile(const String &filepath);
    static Image FromQImage(const QImage &qImage);

private:
    int m_width  = 0;
    int m_height = 0;
    Array<unsigned char> m_pixels8;
    Array<Color> m_pixels;
};

#endif // IMAGE_H
