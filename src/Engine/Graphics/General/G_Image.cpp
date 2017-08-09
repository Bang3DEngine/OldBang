#include "G_Image.h"

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
    if (x < 0 || x >= GetWidth() || y < 0 || y >= GetHeight()) { return; }
    int coord = y * GetWidth() + x;
    m_pixels[coord * 4 + 0] = static_cast<Byte>(color.r * 255);
    m_pixels[coord * 4 + 1] = static_cast<Byte>(color.g * 255);
    m_pixels[coord * 4 + 2] = static_cast<Byte>(color.b * 255);
    m_pixels[coord * 4 + 3] = static_cast<Byte>(color.a * 255);
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

    return Color(m_pixels[y * GetWidth() + x + 0] / 255.0f,
                 m_pixels[y * GetWidth() + x + 1] / 255.0f,
                 m_pixels[y * GetWidth() + x + 2] / 255.0f,
                 m_pixels[y * GetWidth() + x + 3] / 255.0f);
}

uint G_Image::GetWidth() const
{
    return m_size.x;
}

uint G_Image::GetHeight() const
{
    return m_size.y;
}

const Vector2i& G_Image::GetSize() const
{
    return m_size;
}

void G_Image::SaveToFile(const Path &filepath) const
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
    if (ext.IsEmpty()) { ext = "png"; }
    qimg.save(filepath.GetAbsolute().ToQString(), ext.ToCString());
}

G_Image G_Image::FromFile(const Path &filepath)
{
    G_Image img;
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

                int coord = i * img.GetWidth() + j;
                img.m_pixels [coord * 4 + 0] = r;
                img.m_pixels [coord * 4 + 1] = g;
                img.m_pixels [coord * 4 + 2] = b;
                img.m_pixels [coord * 4 + 3] = a;
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

G_Image G_Image::FromQImage(const QImage &qImage)
{
    G_Image img;
    return img;
}
