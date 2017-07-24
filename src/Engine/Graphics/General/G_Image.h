#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>

#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"

class G_Image
{
public:
    G_Image();
    G_Image(int width, int height);

    void Create(int width, int height);
    void Create(int width, int height, const Color& backgroundColor);
    void SetPixel(int x, int y, const Color& color);

    const byte* GetData8() const;
    const Color& GetPixel(int x, int y) const;
    uint GetWidth() const;
    uint GetHeight() const;
    Vector2 GetSize() const;

    void SaveToFile(const Path &filepath) const;
    static G_Image FromFile(const Path &filepath);
    static G_Image FromQImage(const QImage &qImage);

private:
    int m_width  = 0;
    int m_height = 0;
    Array<byte> m_pixels8;
    Array<Color> m_pixels;
};

#endif // IMAGE_H
