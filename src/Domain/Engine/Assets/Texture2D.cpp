#include "Texture2D.h"
#include "Persistence.h"

Texture2D::Texture2D() : Texture(TextureType::Texture2D)
{
}

Texture2D::Texture2D(const std::string &imageFilepath) : Texture2D()
{
    LoadFromFile(imageFilepath);
}

Texture2D::~Texture2D()
{
}

void Texture2D::LoadFromFile(const std::string &imageFilepath)
{
    Bind();
    this->m_filepath = imageFilepath;
    unsigned char *loadedData = FileReader::ReadImage(imageFilepath,
                                                      &m_width, &m_height);
    Fill(loadedData, m_width, m_height);
    UnBind();
}

void Texture2D::CreateEmpty(int width, int height)
{
    Fill(nullptr, width, height);
}

void Texture2D::Resize(int width, int height)
{
    Fill(m_data, width, height);
}

void Texture2D::Fill(unsigned char *newData,
                     int width, int height)
{
    if (this->m_data  && this->m_data != newData)
        delete this->m_data;

    this->m_data = newData;
    this->m_width = width;
    this->m_height = height;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_format, m_internalType, m_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    UnBind();
}

std::string Texture2D::GetImageRelativeFilepath() const
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

    m_filepath = xmlInfo->GetFilepath("TextureFilepath");
    LoadFromFile(m_filepath);

    SetFilterMode(FilterMode_FromString(xmlInfo->GetEnumSelectedName("FilterMode")));
    SetAlphaCuttoff(xmlInfo->GetFloat("AlphaCuttoff"));
}

void Texture2D::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Texture2D");

    int selectedIndex = FilterMode_GetIndexFromValue(GetFilterMode());
    xmlInfo->SetEnum("FilterMode", FilterMode_GetNamesVector(), selectedIndex, {});
    xmlInfo->SetFloat("AlphaCuttoff", GetAlphaCuttoff());
}
