#include "Bang/Texture2D.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/FileReader.h"

Texture2D::Texture2D() : Texture(Target::Texture2D)
{
    CreateEmpty(1,1);
}

Texture2D::Texture2D(const Path &imageFilepath) : Texture2D()
{
    LoadFromImage(imageFilepath);
}

Texture2D::~Texture2D()
{
}

String Texture2D::GetFileExtensionStatic()
{
    return "btex2d";
}

String Texture2D::GetFileExtension() const
{
    return Texture2D::GetFileExtensionStatic();
}


void Texture2D::LoadFromImage(const Path &imageFilepath)
{
    ENSURE(!imageFilepath.IsEmpty());

    m_imageFilepath = imageFilepath;
    Image img = Image::FromFile(m_imageFilepath);
    LoadFromImage(img);
}

void Texture2D::LoadFromImage(const Image &image)
{
    if (image.GetData8())
    {
        m_width  = image.GetWidth();
        m_height = image.GetHeight();

        SetFormat(Texture::Format::RGBA_Byte8);
        Fill(image.GetData8(), m_width, m_height, Texture::Format::RGBA_Byte8);
    }

}

void Texture2D::CreateEmpty(int width, int height)
{
    int dataSize = width * height * Texture::GetPixelBytesSize(m_format);
    byte *data = new byte[dataSize];
    memset(data, 0, dataSize);
    Fill(data, width, height, dataSize, true);
    delete[] data;
}

void Texture2D::Resize(int width, int height)
{
    CreateEmpty(width, height);
}

void Texture2D::Fill(const byte *newData, int width, int height,
                     int sizeOfNewData, bool genMipMaps)
{
    if (m_data) { delete[] m_data; }

    uint dataSize = sizeOfNewData >= 0 ?
                                    sizeOfNewData : (width * height * 16);
    m_data = new byte[dataSize];
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

void Texture2D::GenerateMipMaps() const
{
    Bind();
    glGenerateMipmap(GL_TEXTURE_2D);
    UnBind();
}

void Texture2D::Fill(const byte *newData, int width, int height,
                     Texture::Format imageFormat,
                     bool genMipMaps)
{
    SetFormat(imageFormat);

    int sizeOfNewData =
            width * height * Texture::GetPixelBytesSize(imageFormat);
    Fill(newData, width, height, sizeOfNewData, genMipMaps);
}

const Path& Texture2D::GetImageFilepath() const
{
    return m_imageFilepath;
}

Image Texture2D::ToImage(bool invertY)
{
    const int width  = GetWidth();
    const int height = GetHeight();
    const uint bytesSize = GetBytesSize();
    byte *pixels = new byte[bytesSize];

    glBindTexture(GL_TEXTURE_2D, GetGLId());
    glGetTexImage(GL_TEXTURE_2D,
                  0,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  pixels);
    if (invertY)
    {
        byte *pixelsCpy = new byte[bytesSize];
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

    Image img(width, height);
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

void Texture2D::SetAlphaCutoff(float alphaCutoff)
{
    m_alphaCutoff = alphaCutoff;
}

float Texture2D::GetAlphaCutoff() const
{
    return m_alphaCutoff;
}

void Texture2D::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);

    Path imageFilepath = xmlInfo.GetFilepath("ImageFilepath");
    LoadFromImage(imageFilepath);

    String filterModeString = xmlInfo.GetEnumSelectedName("FilterMode");
    Texture::FilterMode filterMode = FilterMode::Nearest;
    if (filterModeString == "Nearest") { filterMode = FilterMode::Nearest; }
    else if (filterModeString == "Linear") { filterMode = FilterMode::Linear; }
    else if (filterModeString == "Trilinear")
    {
        filterMode = FilterMode::Trilinear;
    }
    SetFilterMode(filterMode);

    SetAlphaCutoff(xmlInfo.GetFloat("AlphaCutoff"));
}

void Texture2D::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    xmlInfo->SetFilepath("ImageFilepath", m_imageFilepath,
                         "jpg png bmp tga tiff gif bmp");

    int selectedIndex = GetFilterMode();
    if (GetFilterMode() == FilterMode::Nearest)        { selectedIndex = 0; }
    else if (GetFilterMode() == FilterMode::Linear)    { selectedIndex = 1; }
    else if (GetFilterMode() == FilterMode::Trilinear) { selectedIndex = 2; }
    xmlInfo->SetEnum("FilterMode", {"Nearest", "Linear", "Trilinear"},
                     selectedIndex, {});

    xmlInfo->SetFloat("AlphaCutoff", GetAlphaCutoff());
}

Texture2D::Texture2D(const Texture2D &t) : Texture2D() {}
