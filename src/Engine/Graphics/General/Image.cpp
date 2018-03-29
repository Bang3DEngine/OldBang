#include "Bang/Image.h"

#include "Bang/Debug.h"
#include "Bang/AARect.h"
#include "Bang/ImageIO.h"
#include "Bang/ImportFilesManager.h"

NAMESPACE_BANG_BEGIN

// Specializations
template<>
void Image<Byte>::Import(const Path &imageFilepath)
{
    bool ok; ImageIO::Import(imageFilepath, this, &ok);
}

template<>
void Image<float>::Import(const Path &imageFilepath)
{
    bool ok;
    Image<Byte> byteImg;
    ImageIO::Import(imageFilepath, &byteImg, &ok);
    *this = byteImg.To<float>();
}

template<>
void Image<Byte>::Export(const Path &filepath) const
{
    ImageIO::Export(filepath, *this);
}

template<>
void Image<float>::Export(const Path &filepath) const
{
    Image<Byte> byteImg = this->To<Byte>();
    ImageIO::Export(filepath, byteImg);
}

template<>
void Image<Byte>::SetPixel(int x, int y, const Color &color)
{
    ASSERT_MSG(x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight(),
               "Pixel (" << x << ", " << y << ") out of bounds");
    const int coord = (y * GetWidth() + x) * 4;
    m_pixels[coord + 0] = Cast<Byte>(color.r * 255);
    m_pixels[coord + 1] = Cast<Byte>(color.g * 255);
    m_pixels[coord + 2] = Cast<Byte>(color.b * 255);
    m_pixels[coord + 3] = Cast<Byte>(color.a * 255);
}

template<>
void Image<float>::SetPixel(int x, int y, const Color &color)
{
    ASSERT_MSG(x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight(),
               "Pixel (" << x << ", " << y << ") out of bounds");
    const int coord = (y * GetWidth() + x) * 4;
    m_pixels[coord + 0] = color.r;
    m_pixels[coord + 1] = color.g;
    m_pixels[coord + 2] = color.b;
    m_pixels[coord + 3] = color.a;
}

template<>
Color Image<Byte>::GetPixel(int x, int y) const
{
    ASSERT_MSG(x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight(),
               "Pixel (" << x << ", " << y << ") out of bounds");
    const std::size_t coord = (y * GetWidth() + x) * 4;
    return Color(m_pixels[coord + 0] / 255.0f,
                 m_pixels[coord + 1] / 255.0f,
                 m_pixels[coord + 2] / 255.0f,
                 m_pixels[coord + 3] / 255.0f);
}

template<>
Color Image<float>::GetPixel(int x, int y) const
{
    ASSERT_MSG(x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight(),
               "Pixel (" << x << ", " << y << ") out of bounds");
    const std::size_t coord = (y * GetWidth() + x) * 4;
    return Color(m_pixels[coord + 0],
                 m_pixels[coord + 1],
                 m_pixels[coord + 2],
            m_pixels[coord + 3]);
}

NAMESPACE_BANG_END
