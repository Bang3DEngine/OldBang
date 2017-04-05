#include "Image.h"

#include "Bang/Debug.h"

Image::Image()
{

}

void Image::SetPixel(int x, int y, const Color& color)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) { return; }
    int coord = y * m_width + x;
    m_pixels [coord]         = color;
    m_pixels8[coord * 4 + 0] = static_cast<unsigned char>(color.r * 255);
    m_pixels8[coord * 4 + 1] = static_cast<unsigned char>(color.g * 255);
    m_pixels8[coord * 4 + 2] = static_cast<unsigned char>(color.b * 255);
    m_pixels8[coord * 4 + 3] = static_cast<unsigned char>(color.a * 255);
}

const unsigned char *Image::GetData8() const
{
    return &m_pixels8[0];
}

const Color &Image::GetPixel(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) { return Color::Zero; }
    return m_pixels[y * m_width + x];
}

unsigned int Image::GetWidth() const
{
    return m_width;
}

unsigned int Image::GetHeight() const
{
    return m_height;
}

Image Image::FromFile(const String &filepath)
{
    Image img;
    QImage qimg(filepath.ToQString());
    if (!qimg.isNull())
    {
        img.m_width  = qimg.width();
        img.m_height = qimg.height();
        img.m_pixels.Resize ( img.GetWidth() * img.GetHeight() );
        img.m_pixels8.Resize( img.GetWidth() * img.GetHeight() * 4);
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
