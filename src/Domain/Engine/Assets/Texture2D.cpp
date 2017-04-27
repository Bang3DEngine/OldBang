#include "Bang/Texture2D.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/FileReader.h"

Texture2D::Texture2D() : Texture(Target::Texture2D)
{
    CreateEmpty(1,1);
}

Texture2D::Texture2D(const String &imageFilepath) : Texture2D()
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


void Texture2D::LoadFromImage(const String &imageFilepath)
{
    ENSURE(!imageFilepath.Empty());

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

String Texture2D::GetImageFilepath() const
{
    return m_imageFilepath;
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

    String imageFilepath = xmlInfo.GetFilepath("ImageFilepath");
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
