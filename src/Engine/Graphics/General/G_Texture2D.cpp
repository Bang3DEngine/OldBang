#include "Bang/G_Texture2D.h"

G_Texture2D::G_Texture2D() : G_Texture(Target::Texture2D)
{
    CreateEmpty(1,1);
}

G_Texture2D::G_Texture2D(const Path &imageFilepath) : G_Texture2D()
{
    LoadFromImage(imageFilepath);
}

G_Texture2D::G_Texture2D(const G_Texture2D &t) : G_Texture(t)
{
}

G_Texture2D::~G_Texture2D()
{
}

void G_Texture2D::LoadFromImage(const Path &imageFilepath)
{
    ENSURE(!imageFilepath.IsEmpty());

    m_imageFilepath = imageFilepath;
    G_Image img = G_Image::FromFile(m_imageFilepath);
    LoadFromImage(img);
}

void G_Texture2D::LoadFromImage(const G_Image &image)
{
    if (image.GetData())
    {
        m_width  = image.GetWidth();
        m_height = image.GetHeight();

        SetFormat(G_Texture::Format::RGBA_Byte8);
        Fill(image.GetData(), m_width, m_height, G_Texture::Format::RGBA_Byte8);
    }

}

void G_Texture2D::CreateEmpty(int width, int height)
{
    int dataSize = width * height * G_Texture::GetPixelBytesSize(m_format);
    Byte *data = new Byte[dataSize];
    memset(data, 0, dataSize);
    Fill(data, width, height, dataSize, true);
    delete[] data;
}

void G_Texture2D::Resize(int width, int height)
{
    CreateEmpty(width, height);
}

void G_Texture2D::Fill(const Byte *newData, int width, int height,
                     int sizeOfNewData, bool genMipMaps)
{
    if (m_data) { delete[] m_data; }

    uint dataSize = sizeOfNewData >= 0 ?
                                    sizeOfNewData : (width * height * 16);
    m_data = new Byte[dataSize];
    memcpy(m_data, newData, dataSize); // Copy data
    m_width = width;
    m_height = height;
    if (!m_data) { m_width = m_height = 0; }

    Bind();
    glTexImage2D(m_target, 0, GetGLInternalFormat(), m_width, m_height, 0,
                 GetGLFormat(), GetGLDataType(), m_data);
    if (genMipMaps && m_width > 0 && m_height > 0)
    {
        GenerateMipMaps();
    }
    UnBind();
}

void G_Texture2D::GenerateMipMaps() const
{
    Bind();
    glGenerateMipmap(GL_TEXTURE_2D);
    UnBind();
}

void G_Texture2D::Fill(const Byte *newData, int width, int height,
                     G_Texture::Format imageFormat,
                     bool genMipMaps)
{
    SetFormat(imageFormat);

    int sizeOfNewData =
            width * height * G_Texture::GetPixelBytesSize(imageFormat);
    Fill(newData, width, height, sizeOfNewData, genMipMaps);
}

const Path& G_Texture2D::GetImageFilepath() const
{
    return m_imageFilepath;
}

G_Image G_Texture2D::ToImage(bool invertY)
{
    const int width  = GetWidth();
    const int height = GetHeight();
    const uint bytesSize = GetBytesSize();
    Byte *pixels = new Byte[bytesSize];

    glBindTexture(GL_TEXTURE_2D, GetGLId());
    glGetTexImage(GL_TEXTURE_2D,
                  0,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  pixels);
    if (invertY)
    {
        Byte *pixelsCpy = new Byte[bytesSize];
        memcpy(pixelsCpy, pixels, bytesSize);
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                const int coords    = (i * width + j) * 4;
                const int invCoords = ((height - i - 1) * width + j) * 4;
                pixels[coords + 0] = pixelsCpy[invCoords + 0];
                pixels[coords + 1] = pixelsCpy[invCoords + 1];
                pixels[coords + 2] = pixelsCpy[invCoords + 2];
                pixels[coords + 3] = pixelsCpy[invCoords + 3];
            }
        }
        delete[] pixelsCpy;
    }

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
