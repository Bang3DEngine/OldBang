#include "Image.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"

Image::Image()
{

}

Image::Image(int width, int height)
{
    Create(width, height, Color::Zero);
}

void Image::Create(int width, int height)
{
    m_width = width;
    m_height = height;
    m_pixels.Resize(m_width * m_height);
    m_pixels8.Resize(m_width * m_height * 4);
}

void Image::Create(int width, int height, const Color &backgroundColor)
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

void Image::SetPixel(int x, int y, const Color& color)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) { return; }
    int coord = y * m_width + x;
    m_pixels [coord]         = color;
    m_pixels8[coord * 4 + 0] = static_cast<byte>(color.r * 255);
    m_pixels8[coord * 4 + 1] = static_cast<byte>(color.g * 255);
    m_pixels8[coord * 4 + 2] = static_cast<byte>(color.b * 255);
    m_pixels8[coord * 4 + 3] = static_cast<byte>(color.a * 255);
}

const byte *Image::GetData8() const
{
    return &m_pixels8[0];
}

const Color &Image::GetPixel(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) { return Color::Zero; }
    return m_pixels[y * m_width + x];
}

uint Image::GetWidth() const
{
    return m_width;
}

uint Image::GetHeight() const
{
    return m_height;
}

Vector2 Image::GetSize() const
{
    return Vector2(GetWidth(), GetHeight());
}

void Image::SaveToFile(const Path &filepath) const
{
    QImage qimg (GetWidth(), GetHeight(), QImage::Format::Format_ARGB32);
    for (int i = 0; i < GetHeight(); ++i)
    {
        for (int j = 0; j < GetWidth(); ++j)
        {
            Color px = GetPixel(j, i) * 255;
            qimg.setPixel(j, i, qRgba(px.r, px.g, px.b, px.a));
        }
    }

    String ext = filepath.GetExtension();
    if (ext.Empty()) { ext = "png"; }
    qimg.save(filepath.GetAbsolute().ToQString(), ext.ToCString());
}

Image Image::FromFile(const Path &filepath)
{
    Image img;
    QImage qimg(filepath.GetAbsolute().ToQString());
    if (!qimg.isNull())
    {
        img.Create(qimg.width(), qimg.height());
        for (int i = 0; i < img.GetHeight(); ++i)
        {
            for (int j = 0; j < img.GetWidth(); ++j)
            {
                QRgb rgb = qimg.pixel(j, i);
                int r = qRed(rgb);
                int g = qGreen(rgb);
                int b = qBlue(rgb);
                int a = qAlpha(rgb);

                int coord = i * img.m_width + j;
                img.m_pixels8 [coord * 4 + 0] = r;
                img.m_pixels8 [coord * 4 + 1] = g;
                img.m_pixels8 [coord * 4 + 2] = b;
                img.m_pixels8 [coord * 4 + 3] = a;
                img.m_pixels  [coord]         = Color(r, g, b, a) / 255.0f;
            }
        }
    }
    else
    {
        Debug_Error("Error loading the image '" << filepath <<
                     "', couldn't open/read the file.");
    }
    return img;
}

Image Image::FromQImage(const QImage &qImage)
{
    Image img;
    return img;
}
