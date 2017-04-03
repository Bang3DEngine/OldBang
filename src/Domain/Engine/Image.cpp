#include "Image.h"

#include "Bang/Debug.h"

Image::Image()
{

}

void Image::ConvertToRGBA8()
{
    Format format = GetFormat();
    if (format == Format::RGBA && GetBytesPerTexel() == 1) { return; }

    QImage img(GetWidth(), GetHeight(), QImage::Format::Format_RGBA8888);
    QImage alphaChannel = img.alphaChannel();
    alphaChannel.fill(QColor::fromRgb(255, 255, 255, 255));
    for (int i = 0; i < GetHeight(); ++i)
    {
        for (int j = 0; j < GetWidth(); ++j)
        {
            img.setPixel( j, i, m_image.pixel(j,i) );
        }
    }
    m_image = img;
    m_image.setAlphaChannel(alphaChannel);
}

const unsigned char *Image::GetData() const
{
    return m_image.bits();
}

unsigned int Image::GetWidth() const
{
    return m_image.width();
}

unsigned int Image::GetHeight() const
{
    return m_image.height();
}

unsigned int Image::GetBitsPerTexel() const
{
    return m_image.depth();
}

unsigned int Image::GetBytesPerTexel() const
{
    return GetBitsPerTexel() / 8;
}

unsigned int Image::GetBytesCount() const
{
    return m_image.byteCount();
}

Image Image::FromFile(const String &filepath)
{
    Image img;
    img.m_image = QImage(filepath.ToQString());
    if (img.m_image.isNull())
    {
        Debug_Error("Error loading the image '" << filepath <<
                     "', couldn't open/read the file.");
    }
    return img;
}

Image Image::FromQImage(const QImage &qImage)
{
    Image img;
    img.m_image = qImage;
    return img;
}


Image::Format Image::GetFormat() const
{
    switch (m_image.format())
    {
        case QImage::Format::Format_ARGB32:
        case QImage::Format::Format_ARGB32_Premultiplied:
            return ARGB;

        case QImage::Format::Format_RGBA8888:
        case QImage::Format::Format_RGBA8888_Premultiplied:
            return RGBA;

        case QImage::Format::Format_RGB16:
        case QImage::Format::Format_RGB30:
        case QImage::Format::Format_RGB32:
        case QImage::Format::Format_RGB444:
        case QImage::Format::Format_RGB555:
        case QImage::Format::Format_RGB666:
        case QImage::Format::Format_RGB888:
            return RGB;
    }
    return RGB;
}
