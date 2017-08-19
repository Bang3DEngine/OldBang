#include "G_Image.h"

#include <QImage>

#include "Bang/Debug.h"

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

const Byte *G_Image::GetData() const
{
    return &m_pixels[0];
}

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
    QImage qimg (GetWidth(), GetHeight(), QImage::Format::Format_ARGB32);
    for (int i = 0; i < GetHeight(); ++i)
    {
        for (int j = 0; j < GetWidth(); ++j)
        {
            Color px = GetPixel(j, i);
            qimg.setPixel(j, i, qRgba(SCAST<Byte>(px.r * 255),
                                      SCAST<Byte>(px.g * 255),
                                      SCAST<Byte>(px.b * 255),
                                      SCAST<Byte>(px.a * 255)));
        }
    }

    String ext = filepath.GetExtension();
    if (ext.IsEmpty()) { ext = "png"; }
    qimg.save(filepath.GetAbsolute().ToQString(), ext.ToCString());
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
    G_Image img;
    QImage qimg(filepath.GetAbsolute().ToQString());
    if (!qimg.isNull())
    {
        img.Create(qimg.width(), qimg.height());
        for (int y = 0; y < img.GetHeight(); ++y)
        {
            for (int x = 0; x < img.GetWidth(); ++x)
            {
                QRgb rgba = qimg.pixel(x, y);
                const int r = qRed(rgba);
                const int g = qGreen(rgba);
                const int b = qBlue(rgba);
                const int a = qAlpha(rgba);

                int coord = (y * img.GetWidth() + x) * 4;
                img.m_pixels [coord + 0] = r;
                img.m_pixels [coord + 1] = g;
                img.m_pixels [coord + 2] = b;
                img.m_pixels [coord + 3] = a;
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
