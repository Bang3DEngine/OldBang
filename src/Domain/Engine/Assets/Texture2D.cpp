#include "Texture2D.h"

#include "Debug.h"
#include "XMLNode.h"
#include "FileReader.h"

Texture2D::Texture2D() : Texture(TextureType::Texture2D)
{
}

Texture2D::Texture2D(const String &imageFilepath) : Texture2D()
{
    LoadFromFile(imageFilepath);
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


void Texture2D::LoadFromFile(const String &imageFilepath)
{
    Bind();
    m_imageFilepath = imageFilepath;
    unsigned char *loadedData = FileReader::ReadImage(m_imageFilepath,
                                                      &m_width, &m_height);
    Fill(loadedData, m_width, m_height);
    UnBind();
}

void Texture2D::CreateEmpty(int width, int height)
{
    int size = width * height;
    const int numChannels = 4, typeSize = 4;
    unsigned char *data = new unsigned char[size * numChannels * typeSize];
    for (int i = 0; i < size; ++i)
    {
        data[i] = 0;
    }
    Fill(data, width, height, false);
}

void Texture2D::Resize(int width, int height)
{
    int size = width * height;
    const int numChannels = 4, typeSize = 4;
    unsigned char *newData = new unsigned char[size * numChannels * typeSize]; //
    Fill(newData, width, height);
}

void Texture2D::Fill(unsigned char *newData,
                     int width, int height, bool genMipMaps)
{
    if (m_data && m_data != newData) { delete[] m_data; }

    m_data = newData; // TODO: Copy (?)
    m_width = width;
    m_height = height;
    if (!m_data) { m_width = m_height = 0; }

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, m_glInternalFormat, m_width, m_height, 0,
                 m_glFormat, m_glType, m_data);
    if (genMipMaps && m_width > 0 && m_height > 0)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    UnBind();
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
    LoadFromFile(imageFilepath);

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
