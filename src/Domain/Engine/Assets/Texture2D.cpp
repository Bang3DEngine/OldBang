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

const String Texture2D::GetFileExtensionStatic()
{
    return "btex2d";
}

const String Texture2D::GetFileExtension()
{
    return Texture2D::GetFileExtensionStatic();
}


void Texture2D::LoadFromFile(const String &imageFilepath)
{
    Bind();
    m_filepath = imageFilepath;
    unsigned char *loadedData = FileReader::ReadImage(imageFilepath,
                                                      &m_width, &m_height);
    Fill(loadedData, m_width, m_height);
    UnBind();
}

void Texture2D::CreateEmpty(int width, int height)
{
    int size = width * height;
    unsigned char *data = new unsigned char[size];
    for (int i = 0; i < size; ++i)
    {
        data[i] = 0;
    }
    Fill(data, width, height, false);
}

void Texture2D::Resize(int width, int height)
{
    Fill(m_data, width, height);
}

void Texture2D::Fill(unsigned char *newData,
                     int width, int height, bool genMipMaps)
{
    if (m_data && m_data != newData) delete m_data;

    m_data = newData; // TODO: Copy (?)
    m_width = width;
    m_height = height;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, m_glInternalFormat, width, height, 0, m_glFormat, m_glType, m_data);
    if (genMipMaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    UnBind();
}

String Texture2D::GetImageRelativeFilepath() const
{
    return m_filepath;
}

void Texture2D::SetAlphaCuttoff(float alphaCuttoff)
{
    m_alphaCuttoff = alphaCuttoff;
}

float Texture2D::GetAlphaCuttoff() const
{
    return m_alphaCuttoff;
}

void Texture2D::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);

    m_filepath = xmlInfo->GetFilepath("ImageFilepath");
    LoadFromFile(m_filepath);

    SetFilterMode(FilterMode_FromString(xmlInfo->GetEnumSelectedName("FilterMode")));
    SetAlphaCuttoff(xmlInfo->GetFloat("AlphaCuttoff"));
}

void Texture2D::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Texture2D");

    xmlInfo->SetFilepath("ImageFilepath", m_filepath, "jpg png bmp");

    int selectedIndex = FilterMode_GetIndexFromValue(GetFilterMode());
    xmlInfo->SetEnum("FilterMode", FilterMode_GetNamesVector(), selectedIndex, {});
    xmlInfo->SetFloat("AlphaCuttoff", GetAlphaCuttoff());
}

Texture2D::Texture2D(const Texture2D &t) : Texture2D() {}
