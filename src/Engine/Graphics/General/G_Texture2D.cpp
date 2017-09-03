#include "Bang/G_Texture2D.h"

#include "Bang/Resources.h"

G_Texture2D::G_Texture2D() : G_Texture(GL::TextureTarget::Texture2D)
{
    CreateEmpty(1,1);
}

G_Texture2D::~G_Texture2D()
{
}

void G_Texture2D::LoadFromImage(const G_Image &image)
{
    if (image.GetData())
    {
        m_width  = image.GetWidth();
        m_height = image.GetHeight();

        SetInternalFormat(GL::ColorFormat::RGBA_UByte8);
        Fill(image.GetData(),
             m_width, m_height,
             GL::ColorComp::RGBA,
             GL::DataType::UnsignedByte);
    }
}

void G_Texture2D::CreateEmpty(int width, int height)
{
    int dataSize = width * height * GL::GetPixelBytesSize(GL::ColorComp::RGB,
                                                          GL::DataType::Byte);
    Byte *data = new Byte[dataSize];
    memset(data, 0, dataSize);
    Fill(nullptr, width, height, GL::ColorComp::RGB, GL::DataType::Byte, true);
    delete[] data;
}

void G_Texture2D::Resize(int width, int height)
{
    CreateEmpty(width, height);
}

void G_Texture2D::Fill(const Color &fillColor,
                       int width, int height,
                       bool genMipMaps)
{
    Array<Color> inputData = Array<Color>(width * height, fillColor);
    Fill( RCAST<const Byte*>(inputData.Data()), width, height,
          GL::ColorComp::RGBA, GL::DataType::Float, genMipMaps);
}

void G_Texture2D::Fill(const Byte *newData,
                       int width, int height,
                       GL::ColorComp inputDataColorComp,
                       GL::DataType inputDataType,
                       bool genMipMaps)
{
    m_width = width;
    m_height = height;

    Bind();
    GL::TexImage2D(m_target, GetWidth(), GetHeight(),
                   GetInternalFormat(),
                   inputDataColorComp, inputDataType,
                   newData);
    if (genMipMaps && m_width > 0 && m_height > 0)
    {
        GenerateMipMaps();
    }
    UnBind();
}

void G_Texture2D::GenerateMipMaps() const
{
    ASSERT(GL::IsBound(this));
    GL::GenerateMipMap( m_target );
}

G_Image G_Texture2D::ToImage(bool invertY)
{
    const int width  = GetWidth();
    const int height = GetHeight();
    const uint bytesSize = GetBytesSize();
    Byte *pixels = new Byte[bytesSize];

    Bind();
    GL::GetTexImage(m_target, pixels);
    UnBind();

    G_Image img(width, height);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            const int coords = (i * width + j) * 4;
            Color pixelColor = Color(pixels[coords + 0] / 255.0f,
                                     pixels[coords + 1] / 255.0f,
                                     pixels[coords + 2] / 255.0f,
                                     pixels[coords + 3] / 255.0f);
            img.SetPixel(j, i, pixelColor);
        }
    }
    if (invertY) { img.InvertVertically(); }

    delete[] pixels;

    return img;
}

void G_Texture2D::SetAlphaCutoff(float alphaCutoff)
{
    m_alphaCutoff = alphaCutoff;
}

float G_Texture2D::GetAlphaCutoff() const
{
    return m_alphaCutoff;
}
