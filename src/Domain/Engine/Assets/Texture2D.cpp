#include "Texture2D.h"

#include "stb_image.h"

#include "Debug.h"
#include "XMLNode.h"
#include "FileReader.h"

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

String Texture2D::GetFileExtension()
{
    return Texture2D::GetFileExtensionStatic();
}


void Texture2D::LoadFromImage(const String &imageFilepath)
{
    m_imageFilepath = imageFilepath;
    unsigned char *loadedData = FileReader::ReadImage(m_imageFilepath,
                                                      &m_width, &m_height);
    SetFormat(Texture::Format::RGBA_Byte8);
    Fill(loadedData, m_width, m_height, Texture::Format::RGBA_Byte8);
    stbi_image_free(loadedData);
}

void Texture2D::CreateEmpty(int width, int height)
{
    int dataSize = width * height * Texture::GetPixelBytesSize(m_format);
    unsigned char *data = new unsigned char[dataSize];
    memset(data, 0, dataSize);
    Fill(data, width, height, dataSize, true);
    delete data;
}

void Texture2D::Resize(int width, int height)
{
    CreateEmpty(width, height);
}

void Texture2D::Fill(unsigned char *newData, int width, int height,
                     int sizeOfNewData, bool genMipMaps)
{
    if (m_data) { delete[] m_data; }

    unsigned int dataSize = sizeOfNewData >= 0 ?
                                    sizeOfNewData : (width * height * 16);
    m_data = new unsigned char[dataSize];
    memcpy(m_data, newData, dataSize); // Copy data
    m_width = width;
    m_height = height;
    if (!m_data) { m_width = m_height = 0; }

    Bind();
    glTexImage2D(m_target, 0, GetGLInternalFormat(),
                 m_width, m_height, 0,
                 GetGLFormat(), GetGLDataType(), m_data);
    if (genMipMaps && m_width > 0 && m_height > 0)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    UnBind();
}

void Texture2D::Fill(unsigned char *newData, int width, int height,
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

void Texture2D::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);

    String imageFilepath = xmlInfo->GetFilepath("ImageFilepath");
    LoadFromImage(imageFilepath);

    String filterModeString = xmlInfo->GetEnumSelectedName("FilterMode");
    Texture::FilterMode filterMode = FilterMode::Nearest;
    if (filterModeString == "Nearest") { filterMode = FilterMode::Nearest; }
    else if (filterModeString == "Linear") { filterMode = FilterMode::Linear; }
    else if (filterModeString == "Trilinear")
    {
        filterMode = FilterMode::Trilinear;
    }
    SetFilterMode(filterMode);

    SetAlphaCutoff(xmlInfo->GetFloat("AlphaCutoff"));
}

void Texture2D::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Texture2D");

    xmlInfo->SetFilepath("ImageFilepath", m_imageFilepath, "jpg png bmp");

    int selectedIndex = GetFilterMode();
    if (GetFilterMode() == FilterMode::Nearest)        { selectedIndex = 0; }
    else if (GetFilterMode() == FilterMode::Linear)    { selectedIndex = 1; }
    else if (GetFilterMode() == FilterMode::Trilinear) { selectedIndex = 2; }
    xmlInfo->SetEnum("FilterMode", {"Nearest", "Linear", "Trilinear"},
                     selectedIndex, {});

    xmlInfo->SetFloat("AlphaCutoff", GetAlphaCutoff());
}

Texture2D::Texture2D(const Texture2D &t) : Texture2D() {}
