#include "G_Image.h"

#include "Bang/Debug.h"
#include "Bang/ImageIO.h"

G_Image::G_Image()
{

}

G_Image::G_Image(int width, int height)
{
    Create(width, height);
}

void G_Image::Create(int width, int height)
{
    m_size = Vector2i(width, height);
    m_pixels.Resize(m_size.x * m_size.y * 4);
}

void G_Image::Create(int width, int height, const Color &backgroundColor)
{
    Create(width, height);
    for (int i = 0; i < GetHeight(); ++i)
    {
        for (int j = 0; j < GetWidth(); ++j)
        {
            SetPixel(j, i, backgroundColor);
        }
    }
}

void G_Image::SetPixel(int x, int y, const Color& color)
{
    if (x < 0 || x >= GetWidth() || y < 0 || y >= GetHeight())
    {
        Debug_Warn("Pixel (" << x << ", " << y << ") out of range.");
        return;
    }

    const int coord = (y * GetWidth() + x) * 4;
    m_pixels[coord + 0] = SCAST<Byte>(color.r * 255);
    m_pixels[coord + 1] = SCAST<Byte>(color.g * 255);
    m_pixels[coord + 2] = SCAST<Byte>(color.b * 255);
    m_pixels[coord + 3] = SCAST<Byte>(color.a * 255);
}

Byte *G_Image::GetData() { return &m_pixels[0]; }
const Byte *G_Image::GetData() const { return &m_pixels[0]; }

Color G_Image::GetPixel(int x, int y) const
{
    if (x < 0 || x >= GetWidth() || y < 0 || y >= GetHeight())
    {
        Debug_Warn("Pixel (" << x << ", " << y << ") out of range.");
        return Color::Zero;
    }

    const int coord = (y * GetWidth() + x) * 4;
    return Color(m_pixels[coord + 0] / 255.0f,
                 m_pixels[coord + 1] / 255.0f,
                 m_pixels[coord + 2] / 255.0f,
                 m_pixels[coord + 3] / 255.0f);
}

int G_Image::GetWidth() const { return m_size.x; }
int G_Image::GetHeight() const { return m_size.y; }
const Vector2i& G_Image::GetSize() const { return m_size; }

void G_Image::InvertVertically()
{
    G_Image img = *this;
    for (int y = 0; y < GetHeight(); ++y)
    {
        for (int x = 0; x < GetWidth(); ++x)
        {
            Color c = img.GetPixel(x, y);
            SetPixel(x, GetHeight() - y - 1, c);
        }
    }
}

void G_Image::SaveToFile(const Path &filepath) const
{
    ImageIO::Write(filepath, *this);
}

G_Image G_Image::LoadFromData(int width, int height,
                              const Array<Byte> &rgbaByteData)
{
    G_Image img(width, height);
    img.m_pixels = rgbaByteData;
    ASSERT(rgbaByteData.Size() == (img.GetWidth() * img.GetHeight() * 4));
    return img;
}

G_Image G_Image::LoadFromFile(const Path &filepath)
{
    bool ok;
    G_Image img;
    ImageIO::Read(filepath, &img, &ok);

    if (!ok)
    {
        Debug_Error("Error loading the image '" << filepath <<
                     "', couldn't open/read the file.");
    }
    return img;
}
